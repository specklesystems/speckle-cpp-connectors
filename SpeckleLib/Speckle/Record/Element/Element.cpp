#include "Speckle/Record/Element/Element.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/Guid.h"

#ifdef ARCHICAD
#include "Sight.hpp"
#include "Model.hpp"
#include "ModelMaterial.hpp"
#include "ModelElement.hpp"
#include "exp.h"
#include "ModelMeshBody.hpp"
#include "ConvexPolygon.hpp"
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

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{ std::make_unique<API_Element>(elem) } {}
		Data(const Data& source) : root{ std::make_unique<API_Element>(*source.root) } {}
#endif

	private:
		std::unique_ptr<API_Element> root;
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

	int32_t ARGBToInt(double alpha, double red, double green, double blue) {
		// Convert double (0.0 - 1.0) to uint8_t (0 - 255)
		uint8_t a = static_cast<uint8_t>(std::round(alpha * 255.0));
		uint8_t r = static_cast<uint8_t>(std::round(red * 255.0));
		uint8_t g = static_cast<uint8_t>(std::round(green * 255.0));
		uint8_t b = static_cast<uint8_t>(std::round(blue * 255.0));

		// Pack ARGB into a single 32-bit integer
		return (a << 24) | (r << 16) | (g << 8) | b;
	}
}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Element::Element() {
} //Element::Element


/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The attribute table ID (attribute type)
  --------------------------------------------------------------------*/
Element::Element(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //Element::Element


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Element::Element(const Element& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
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
const API_Elem_Head& Element::getHead() const {
	return m_data->root->header;
} //Element::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Element::getHead() {
	return m_data->root->header;
} //Element::getHead
#endif


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
