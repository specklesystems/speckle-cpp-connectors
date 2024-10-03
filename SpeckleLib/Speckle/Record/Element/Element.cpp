#include "Speckle/Record/Element/Element.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {
	
	class Element::Data {
	public:
		friend class Element;
		
		Data(const API_Element& elem) : root{std::make_unique<API_Element>(elem)} {}
		Data(const Data& source) : root{std::make_unique<API_Element>(*source.root)} {}
		
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

	void GetComponent(API_Component3D& component, API_3DTypeID typeId, Int32 index) {
		component.header.typeID = typeId;
		component.header.index = index;
		if (ACAPI_ModelAccess_GetComponent(&component) != NoError) {
			// TODO: throw
		}
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
  --------------------------------------------------------------------*/
Element::Element(const API_Element& elemData) {
	m_data = std::make_unique<Data>(elemData);
} //Element::Element


/*--------------------------------------------------------------------
	Copy constructor
 
 	source: The object to copy
  --------------------------------------------------------------------*/
Element::Element(const Element& source) {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Element::Element


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Element::~Element() {}

Element::Body* Element::getBody() const {

	if (m_data->m_cache) {
		return m_data->m_cache.get();
	}

	auto elementBody = new Element::Body();
	std::map<int, int> vertexIndexMap;
	int currentVertexIndex = 0;

	API_ElemInfo3D info3D = {};

	if (ACAPI_ModelAccess_Get3DInfo(getHead(), &info3D) != NoError) {
		// TODO: throw
	}

	for (Int32 ib = info3D.fbody; ib <= info3D.lbody; ib++) {
		API_Component3D component = {};
		GetComponent(component, API_BodyID, ib);

		std::vector<double> vertices;
		std::vector<int> faces;
		std::vector<int> colors;

		vertices.clear();
		faces.clear();
		colors.clear();

		Int32 nPgon = component.body.nPgon;
		for (Int32 ip = 1; ip <= nPgon; ip++) {
			GetComponent(component, API_PgonID, ip);

			Int32 fpedg = component.pgon.fpedg;
			Int32 lpedg = component.pgon.lpedg;
			Int32 faceSize = lpedg - fpedg + 1;
			for (Int32 ie = fpedg; ie <= lpedg; ie++)
			{
				GetComponent(component, API_PedgID, ie);

				// TODO is this needed? need review, not sure how ACAPI_ModelAccess works
				bool wasNegative = component.pedg.pedg < 0;
				Int32 edgeIndex = std::abs(component.pedg.pedg);
				GetComponent(component, API_EdgeID, edgeIndex);

				// TODO is this needed? need review, not sure how ACAPI_ModelAccess works
				Int32 vertexIndex = wasNegative ? component.edge.vert2 : component.edge.vert1;
				/*auto materialIndex = component.pgon.iumat;
				GetComponent(component, API_UmatID, materialIndex);
				double R = component.umat.mater.surfaceRGB.f_red;
				double G = component.umat.mater.surfaceRGB.f_green;
				double B = component.umat.mater.surfaceRGB.f_blue;*/
				// TODO: other material stuff

				GetComponent(component, API_VertID, vertexIndex);

				faces.push_back(faceSize);
				if (vertexIndexMap.find(vertexIndex) == vertexIndexMap.end()) {
					faces.push_back(currentVertexIndex);
					vertexIndexMap[vertexIndex] = currentVertexIndex++;

					vertices.push_back(component.vert.x);
					vertices.push_back(component.vert.y);
					vertices.push_back(component.vert.z);
				}
				else {
					faces.push_back(vertexIndexMap[vertexIndex]);
				}
			}
		}
		elementBody->push_back(primitive::Mesh(std::move(vertices), std::move(faces), std::move(colors)));
	}
	m_data->m_cache.reset(elementBody);
	return m_data->m_cache.get();
}


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


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Element::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	auto body = getBody();
	inventory.merge(Inventory{
		{
			{ fieldID[bodyID], bodyID, body == nullptr ? 0 : static_cast<uint32_t>(body->size()) },	//TODO: implement other fields
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
				//return std::make_unique<active::serialise::ContainerWrap>(*body);
				return Cargo::Unique{ new active::serialise::ContainerWrap{ *body } };
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
