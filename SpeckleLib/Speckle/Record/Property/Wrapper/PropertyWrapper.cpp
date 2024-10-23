#include "Speckle/Record/Property/Wrapper/PropertyWrapper.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Record/Property/Property.h"

#include <array>

using namespace active::serialise;
using namespace speckle::record::property;
using namespace speckle::utility;

using enum Entry::Type;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		nameID,
		groupID,
		valueID,
		applicID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
		Identity{"group"},
		Identity{"value"},
		Identity{"applicationID"},
	};

}


// MARK: - Constructors

/*--------------------------------------------------------------------
	Constructor
 
	point: The point to wrap for (de)serialisation
  --------------------------------------------------------------------*/
PropertyWrapper::PropertyWrapper(const Property& property) :
		m_name{property.getName()}, m_groupName{property.getGroupName()}, m_displayValue{property.getDisplayValue()}, m_ID{property.getTemplateID()} {
} //PropertyWrapper::PropertyWrapper


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool PropertyWrapper::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
			{ fieldID[groupID], groupID, element, !m_groupName.empty() },
			{ fieldID[valueID], valueID, element },
			{ fieldID[applicID], applicID, element, m_ID.operator bool() },
		},
	}.withType(&typeid(PropertyWrapper)));
	return true;
} //PropertyWrapper::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique PropertyWrapper::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(PropertyWrapper))
		return nullptr;
	switch (item.index) {
		case FieldIndex::nameID:
			return std::make_unique<StringWrap>(m_name);
		case FieldIndex::groupID:
			return std::make_unique<StringWrap>(m_groupName);
		case FieldIndex::valueID:
			return std::make_unique<StringWrap>(m_displayValue);
		case FieldIndex::applicID:
			return std::make_unique<ValueWrap<Guid>>(m_ID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //PropertyWrapper::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void PropertyWrapper::setDefault() {
	m_name.clear();
	m_groupName.clear();
	m_displayValue.clear();
	m_ID.clear();
} //PropertyWrapper::setDefault
