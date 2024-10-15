#include "Connector/Interface/Browser/Bridge/Selection/Arg/SelectionInfo.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;

namespace {

		///Serialisation fields
	enum FieldIndex {
		summaryID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"summary"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SelectionInfo::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[summaryID], summaryID, element },
		},
	}.withType(&typeid(SelectionInfo)));
	return true;
} //ConnectorConfig::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SelectionInfo::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(SelectionInfo))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case summaryID:
			return std::make_unique<ValueWrap<active::utility::String>>(summary);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ConnectorConfig::getCargo
