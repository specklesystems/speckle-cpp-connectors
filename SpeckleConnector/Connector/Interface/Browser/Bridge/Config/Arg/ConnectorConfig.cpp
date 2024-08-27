#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/PackageWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;

namespace {

		///Serialisation fields
	enum FieldIndex {
		arg0,
		darkTheme,
		darkThemeAlt,	///>This recent addition might not be permanent - watch for changes
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"0"},
		Identity{"DarkTheme"},
		Identity{"darkTheme"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ConnectorConfig::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//When used as an argument template, this wrapper is expected to be first in the list
	if (isArgumentTemplate())
		inventory.merge({ fieldID[arg0], arg0, element, true, &typeid(ConnectorConfig) });
	inventory.merge(Inventory{
		{
			{ fieldID[darkTheme], darkTheme, element },
			{ fieldID[darkThemeAlt], darkThemeAlt, element },
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
		case arg0:
				//This structure is the first argument
			return std::make_unique<PackageWrap>(*this);
		case darkTheme: case darkThemeAlt:
			return std::make_unique<ValueWrap<bool>>(isDarkTheme);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ConnectorConfig::getCargo
