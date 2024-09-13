#include "Connector/Record/Model/Filter/SendFilter.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		nameID,
		summaryID,
		defaultID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
		Identity{"summary"},
		Identity{"isDefault"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SendFilter::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
			{ fieldID[summaryID], summaryID, element },
			{ fieldID[defaultID], defaultID, element },
		},
	}.withType(&typeid(SendFilter)));
	return true;
} //SendFilter::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SendFilter::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(SendFilter))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			return std::make_unique<ValueWrap<String>>(m_name);
		case summaryID:
			return std::make_unique<ValueWrap<String>>(m_summary);
		case defaultID:
			return std::make_unique<ValueWrap<bool>>(m_isDefault);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendFilter::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void SendFilter::setDefault() {
	m_name.clear();
	m_summary.clear();
	m_isDefault = false;
} //SendFilter::setDefault
