#include "Connector/Interface/Browser/Bridge/Send/Arg/SendObject.h"

#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::serialise;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		idID,
		rootObjID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"id"},
		Identity{"rootObject"},
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
			{ fieldID[rootObjID], rootObjID, element },
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
		case rootObjID:
			return std::make_unique<PackageWrap>(base::get());
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendObject::getCargo
