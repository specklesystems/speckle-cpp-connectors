#include "Speckle/Record/Element/GenericElement.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class GenericElement::Data {
	public:
		friend class GenericElement;

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{ std::make_unique<API_Element>(elem) } {}
		Data(const Data& source) : root{ std::make_unique<API_Element>(*source.root) } {}
#endif

	private:
		std::unique_ptr<API_Element> root;
		std::unique_ptr<GenericElement::Body> m_cache;
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
GenericElement::GenericElement() {
} //GenericElement::GenericElement


/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The attribute table ID (attribute type)
  --------------------------------------------------------------------*/
GenericElement::GenericElement(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //GenericElement::GenericElement


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
GenericElement::GenericElement(const GenericElement& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //GenericElement::GenericElement


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
GenericElement::~GenericElement() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& GenericElement::getHead() const {
	return m_data->root->header;
} //GenericElement::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& GenericElement::getHead() {
	return m_data->root->header;
} //GenericElement::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool GenericElement::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[bodyID], bodyID, element },	//TODO: implement other fields
		},
	}.withType(&typeid(GenericElement)));
	return base::fillInventory(inventory);
} //GenericElement::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique GenericElement::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(GenericElement))
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
} //GenericElement::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void GenericElement::setDefault() {
	base::setDefault();
	m_data.reset();
} //GenericElement::setDefault
