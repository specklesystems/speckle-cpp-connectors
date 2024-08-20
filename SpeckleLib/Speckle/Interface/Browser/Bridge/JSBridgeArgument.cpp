#include "Speckle/Interface/Browser/Bridge/JSBridgeArgument.h"

using namespace active::serialise;
using namespace speckle::interfac::browser::bridge;


/*--------------------------------------------------------------------
	Fill an inventory with the cargo items
 
	inventory: The inventory to receive the cargo items
 
	return: True if items have been added to the inventory
  --------------------------------------------------------------------*/
bool JSBridgeArgument::fillInventory(active::serialise::Inventory& inventory) const {
	return true;
} //JSBridgeArgument::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique JSBridgeArgument::getCargo(const active::serialise::Inventory::Item& item) const {
	return nullptr;
} //JSBridgeArgument::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void JSBridgeArgument::setDefault() {
} //JSBridgeArgument::setDefault
