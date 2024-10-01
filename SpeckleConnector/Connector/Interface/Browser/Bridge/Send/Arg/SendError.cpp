#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"

#include "Active/Serialise/item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		errorID,
		cardID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"error"},
		Identity{"modelCardId"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SendError::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[errorID], errorID, element },
			{ fieldID[cardID], cardID, element },
		},
	}.withType(&typeid(SendError)));
	return true;
} //SendError::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SendError::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(SendError))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case errorID:
			return std::make_unique<ValueWrap<String>>(message);
		case cardID:
			return std::make_unique<ValueWrap<String>>(modelCardID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendError::getCargo
