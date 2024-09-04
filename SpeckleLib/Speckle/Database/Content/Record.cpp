#include "Speckle/Database/Content/Record.h"

#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Record::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ Identity{"id"}, active::database::record::FieldIndex::idIndex, element },
		},
	}.withType(&typeid(base)));
	return true;
} //Record::fillInventory
