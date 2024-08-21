#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;

namespace {

		///Serialisation fields
	enum FieldIndex {
		darkTheme,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"DarkTheme"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ConnectorConfig::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[darkTheme], darkTheme, element },
		},
	}.withType(&typeid(ConnectorConfig)));
	return true;
} //ConnectorConfig::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ConnectorConfig::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ConnectorConfig))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case darkTheme:
			return std::make_unique<ValueWrap<bool>>(isDarkTheme);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ConnectorConfig::getCargo
