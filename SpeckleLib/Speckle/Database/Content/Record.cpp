#include "Speckle/Database/Content/Record.h"

#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Fill an inventory with the package items
	@param inventory The inventory to receive the package items
	@return True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Record::fillInventory(Inventory& inventory) const {
		//TODO: Complete
	return true;
} //Record::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
	@param item The inventory item to retrieve
	@return The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Record::getCargo(const Inventory::Item& item) const {
		//TODO: Complete
	return nullptr;
} //Record::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Record::setDefault() {
	//TODO: Complete
} //Record::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool Record::validate() {
	//TODO: Complete
	return true;
} //Record::validate
