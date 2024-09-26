#include "Speckle/Record/Element/Element.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>

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
			return nullptr;	//TODO: implement
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
