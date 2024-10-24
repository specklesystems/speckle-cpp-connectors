#include "Speckle/Record/Element/ModelElement.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Record/Property/Wrapper/PropertiedWrapper.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/Guid.h"

#ifdef ARCHICAD
#include <Sight.hpp>
#include <Model.hpp>
#include <ModelMaterial.hpp>
#include <ModelElement.hpp>
#include <exp.h>
#include <ModelMeshBody.hpp>
#include <ConvexPolygon.hpp>
#endif

using namespace active::measure;
using namespace active::serialise;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::record::property;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class ModelElement::Data {
	public:
		friend class ModelElement;

#ifdef ARCHICAD
		Data() {}
		Data(const Data& source) : m_cache{std::make_unique<ModelElement::Body>(*source.m_cache)} {}
#endif

	private:
		std::unique_ptr<ModelElement::Body> m_cache;
	};

}

namespace {

	///Serialisation fields
	enum FieldIndex {
		bodyID,
		propertyID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"displayValue"},
		Identity{"properties"},
	};
	
}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ModelElement::ModelElement() {
} //ModelElement::ModelElement


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
ModelElement::ModelElement(const Guid& ID, const Guid& tableID, std::optional<LengthType> unit) : Element{ID, tableID, unit} {
} //ModelElement::ModelElement


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
ModelElement::ModelElement(const ModelElement& source) : base{source}, Classified{source}, Propertied{source} {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //ModelElement::ModelElement


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ModelElement::~ModelElement() {}


/*--------------------------------------------------------------------
	Get the element body as a list of faces or Meshes

	return: A pointer to the element body
  --------------------------------------------------------------------*/
ModelElement::Body* ModelElement::getBody() const {
#ifdef ARCHICAD
	if (m_data && m_data->m_cache) {
		return m_data->m_cache.get();
	}

	void* dummy = nullptr;
	GSErrCode err = ACAPI_Sight_GetCurrentWindowSight(&dummy);
	if (err != NoError)
	{
		// TODO: should this throw?
	}

	Modeler::SightPtr currentSightPtr((Modeler::Sight*)dummy); // init the shared ptr with the raw pointer
	ModelerAPI::Model acModel;
	Modeler::IAttributeReader* attrReader = ACAPI_Attribute_GetCurrentAttributeSetReader();

	err = EXPGetModel(currentSightPtr, &acModel, attrReader);
	if (err != NoError)
	{
		// TODO: should this throw?
	}

	auto elementBody = new ModelElement::Body();

	// Map to collect meshes per material name
	std::map<GS::UniString, primitive::Mesh> materialMeshMap;

	Int32 nElements = acModel.GetElementCount();
	for (Int32 iElement = 1; iElement <= nElements; iElement++)
	{
		ModelerAPI::Element elem{};
		acModel.GetElement(iElement, &elem);
		if (elem.GetElemGuid() != getHead().guid)
			continue;

		Int32 nBodies = elem.GetTessellatedBodyCount();
		for (Int32 bodyIndex = 1; bodyIndex <= nBodies; ++bodyIndex)
		{
			ModelerAPI::MeshBody body{};
			elem.GetTessellatedBody(bodyIndex, &body);

			Int32 polyCount = body.GetPolygonCount();
			for (Int32 polyIndex = 1; polyIndex <= polyCount; ++polyIndex)
			{
				ModelerAPI::Polygon polygon{};
				body.GetPolygon(polyIndex, &polygon);

				ModelerAPI::Material material{};
				polygon.GetMaterial(&material);
				auto materialName = material.GetName();
				if (materialMeshMap.find(materialName) == materialMeshMap.end()) {
					materialMeshMap[materialName] = primitive::Mesh(material);
				}

				Int32 convexPolyCount = polygon.GetConvexPolygonCount();

				for (Int32 convPolyIndex = 1; convPolyIndex <= convexPolyCount; ++convPolyIndex)
				{
					std::vector<double> vertices;
					ModelerAPI::ConvexPolygon convexPolygon{};
					polygon.GetConvexPolygon(convPolyIndex, &convexPolygon);
					Int32 vertexCount = convexPolygon.GetVertexCount();

					for (Int32 vertexIndex = 1; vertexIndex <= vertexCount; ++vertexIndex)
					{
						ModelerAPI::Vertex vertex{};
						body.GetVertex(convexPolygon.GetVertexIndex(vertexIndex), &vertex);

						// Collect vertices (as doubles for now, but should be changed to Vertex type)
						vertices.push_back(vertex.x);
						vertices.push_back(vertex.y);
						vertices.push_back(vertex.z);
					}

					materialMeshMap[materialName].appendFace(std::move(vertices));
				}
			}
		}
	}
	
	for (auto& [materialName, mesh] : materialMeshMap)
	{
		elementBody->push_back(std::move(mesh));
	}
	m_data = std::make_unique<Data>();
	m_data->m_cache.reset(elementBody);
	return m_data->m_cache.get();
#endif
} //ModelElement::getBody


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ModelElement::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[bodyID], bodyID, element },
			{ fieldID[propertyID], propertyID, element },
		},
	}.withType(&typeid(ModelElement)));
	return base::fillInventory(inventory);
} //ModelElement::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ModelElement::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ModelElement))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case bodyID:
			if (auto body = getBody(); body != nullptr)
				return Cargo::Unique{ new active::serialise::ContainerWrap{*body} };
			else
				return nullptr;
		case propertyID:
			return std::make_unique<PropertiedWrapper>(*this);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ModelElement::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ModelElement::setDefault() {
	base::setDefault();
	m_data.reset();
} //ModelElement::setDefault
