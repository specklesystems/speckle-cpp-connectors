#include "Speckle/Record/Credentials/ServerMigration.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::record::cred;
using namespace speckle::utility;

#include <array>

namespace {

		///Serialisation fields
	enum FieldIndex {
		movedToID,
		movedFromID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"movedTo"},
		Identity{"movedFrom"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ServerMigration::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[movedToID], movedToID, element },
			{ fieldID[movedFromID], movedFromID, element },
		},
	}.withType(&typeid(ServerMigration)));
	return true;
} //ServerMigration::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ServerMigration::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ServerMigration))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case movedToID:
			return std::make_unique<ValueWrap<String>>(movedTo);
		case movedFromID:
			return std::make_unique<ValueWrap<String>>(movedFrom);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ServerMigration::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ServerMigration::setDefault() {
	movedTo.clear();
	movedFrom.clear();
} //ServerMigration::setDefault
