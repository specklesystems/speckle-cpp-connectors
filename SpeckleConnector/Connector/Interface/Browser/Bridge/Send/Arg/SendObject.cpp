#include "Connector/Interface/Browser/Bridge/Send/Arg/SendObject.h"

#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::serialise;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		idID,
		totChildID,
		batchesID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"error"},
		Identity{"totalChildrenCount"},
		Identity{"batches"},
	};
	
}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SendObject::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[idID], idID, element },
			{ fieldID[totChildID], totChildID, element },
			{ fieldID[batchesID], batchesID, element },
		},
	}.withType(&typeid(SendObject)));
	return true;
} //SendObject::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SendObject::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(SendObject))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case idID:
			return std::make_unique<StringWrap>(id);
		case totChildID:
			return std::make_unique<Int32Wrap>(totalChildrenCount);
		case batchesID:
			return std::make_unique<ContainerWrap<std::vector<speckle::utility::String>>>(batches);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendObject::getCargo
