#include "Connector/Record/Collection/RootCollection.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool RootCollection::fillInventory(Inventory& inventory) const {
		//Extend with supplementary data as required
	return base::fillInventory(inventory);
} //RootCollection::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique RootCollection::getCargo(const Inventory::Item& item) const {
		//Extend with supplementary data as required
	return base::getCargo(item);
} //RootCollection::getCargo
