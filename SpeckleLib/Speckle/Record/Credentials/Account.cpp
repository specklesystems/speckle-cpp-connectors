#include "Speckle/Record/Credentials/Account.h"

#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::record::cred;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Account::Account(Guid ID) : Record{ID} {
	//TODO: Complete
} //Account::Account


/*--------------------------------------------------------------------
	Fill an inventory with the package items
	@param inventory The inventory to receive the package items
	@return True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Account::fillInventory(Inventory& inventory) const {
		//TODO: Complete
	return true;
} //Account::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
	@param item The inventory item to retrieve
	@return The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Account::getCargo(const Inventory::Item& item) const {
		//TODO: Complete
	return nullptr;
} //Account::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Account::setDefault() {
	//TODO: Complete
} //Account::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool Account::validate() {
	//TODO: Complete
	return true;
} //Account::validate
