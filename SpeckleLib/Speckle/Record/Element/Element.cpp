#include "Speckle/Record/Element/Element.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Record/Element/Memo.h"
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

using namespace active::serialise;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class Element::Data {
	public:
		friend class Element;
		Data() {}
		Data(const Data& source) {}

	private:
		std::unique_ptr<Element::Body> m_cache;
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
Element::Element() {
} //Element::Element


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
Element::Element(const Guid& ID, const Guid& tableID, std::optional<active::measure::LengthType> unit) : base{ID, tableID, unit} {
	m_data = std::make_unique<Data>();
} //Element::Element


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Element::Element(const Element& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Element::Element


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Element::Element(Element&& source) : base{source} {
	m_data = std::move(source.m_data);
} //Element::Element


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Element::~Element() {}


/*--------------------------------------------------------------------
	Get the element storey

	return: The element storey (nullopt if the element isn't linked to a storey)
  --------------------------------------------------------------------*/
Storey::Option Element::getStorey() const {
#ifdef ARCHICAD
	return Storey{ getHead().floorInd };
#endif
} //Element::getStorey


/*--------------------------------------------------------------------
	Get the element type name, e.g. "Wall", "Roof" etc

	return: The type name
  --------------------------------------------------------------------*/
String Element::getTypeName() const {
#ifdef ARCHICAD
	GS::UniString typeName;
	if (auto err = ACAPI_Element_GetElemTypeName(getHead().type, typeName); err != NoError)
		return addon()->getLocalString(titleStringLib, unknownElementTypeID);
	return typeName;
#endif
} //Element::getTypeName


/*--------------------------------------------------------------------
	Get the element body as a list of faces or Meshes

	return: A pointer to the element body
  --------------------------------------------------------------------*/
Element::Body* Element::getBody() const {
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

	auto elementBody = new Element::Body();

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

	m_data->m_cache.reset(elementBody);
	return m_data->m_cache.get();
#endif
}



/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Element::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[bodyID], bodyID, element },	//TODO: implement other fields
		},
	}.withType(&typeid(Element)));
	return base::fillInventory(inventory);
} //Element::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Element::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Element))
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
} //Element::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Element::setDefault() {
	base::setDefault();
	m_data.reset();
} //Element::setDefault


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Element::loadMemo(Part::filter_bits filter, std::unique_ptr<Memo>& memo) const {
		//If the memo data isn't loaded, fetch it now
	if (!memo) {
		auto project = addon()->getActiveProject().lock();
		if (!project)
			return;
		if (auto loaded = project->getElementDatabase()->getMemo(getBIMID(), filter); loaded)
			memo.reset(loaded.release());
	}
} //Element::loadMemo
