#include "Speckle/Database/Content/BIMRecord.h"

#include "Speckle/Utility/Guid.h"

#include <array>

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		applicID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"applicationId"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool BIMRecord::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ Identity{fieldID[applicID]}, applicID, element },
		},
	}.withType(&typeid(BIMRecord)));
	return base::fillInventory(inventory);
} //BIMRecord::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique BIMRecord::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(BIMRecord))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case applicID:
			return std::make_unique<ValueWrap<BIMRecordID>>(m_applicationID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //BIMRecord::getCargo
