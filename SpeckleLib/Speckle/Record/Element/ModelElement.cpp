#include "Speckle/Record/Element/ModelElement.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
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
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class ModelElement::Data {
	public:
		friend class ModelElement;

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{ std::make_unique<API_Element>(elem) } {}
		Data(const Data& source) : root{ std::make_unique<API_Element>(*source.root) } {}
#endif

	private:
		std::unique_ptr<API_Element> root;
		std::unique_ptr<ModelElement::Body> m_cache;
	};

}

namespace {

	///Serialisation fields
	enum FieldIndex {
		bodyID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"displayValue"},
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
	Constructor

	elemData: Archicad element data
	tableID: The attribute table ID (attribute type)
  --------------------------------------------------------------------*/
ModelElement::ModelElement(const API_Element& elemData, const Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
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
	if (m_data->m_cache) {
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
				ModelerAPI::Polygon	polygon{};
				body.GetPolygon(polyIndex, &polygon);

				ModelerAPI::Material material{};
				polygon.GetMaterial(&material);
				Int32 convexPolyCount = polygon.GetConvexPolygonCount();

				for (Int32 convPolyIndex = 1; convPolyIndex <= convexPolyCount; ++convPolyIndex)
				{
					std::vector<double> vertices;
					std::vector<int> faces;
					std::vector<int> colors;

					ModelerAPI::ConvexPolygon convexPolygon{};
					polygon.GetConvexPolygon(convPolyIndex, &convexPolygon);
					Int32 vertexCount = convexPolygon.GetVertexCount();

					faces.push_back(vertexCount);
					for (Int32 vertexIndex = 1; vertexIndex <= vertexCount; ++vertexIndex)
					{
						ModelerAPI::Vertex vertex{};
						body.GetVertex(convexPolygon.GetVertexIndex(vertexIndex), &vertex);

						// TODO: change vertices array to hold Vertex instead of double values
						vertices.push_back(vertex.x);
						vertices.push_back(vertex.y);
						vertices.push_back(vertex.z);

						//double alpha = material.GetTransparency();
						//ModelerAPI::Color color = material.GetSurfaceColor();
						//colors.push_back(ARGBToInt(alpha, color.red, color.green, color.blue));

						faces.push_back(vertexIndex - 1);
					}
					elementBody->push_back(primitive::Mesh(std::move(vertices), std::move(faces), std::move(colors), material));
				}
			}
		}
	}
	m_data->m_cache.reset(elementBody);
	return m_data->m_cache.get();
#endif
}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& ModelElement::getHead() const {
	return m_data->root->header;
} //ModelElement::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& ModelElement::getHead() {
	return m_data->root->header;
} //ModelElement::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ModelElement::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[bodyID], bodyID, element },	//TODO: implement other fields
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
		{
			return Cargo::Unique{ new active::serialise::ContainerWrap{*body} };
		}
		else
			return nullptr;

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
