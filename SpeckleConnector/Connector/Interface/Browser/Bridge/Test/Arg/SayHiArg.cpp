#include "Connector/Interface/Browser/Bridge/Test/Arg/SayHiArg.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		nameID,
		countID,
		isHelloID,
	};

		///Serialisation field IDs. NB: This structure is never exported, so we only define the incoming argument indices
	static std::array fieldID = {
		Identity{"0"},
		Identity{"1"},
		Identity{"2"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SayHiArg::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
			{ fieldID[countID], countID, element },
			{ fieldID[isHelloID], isHelloID, element },
		},
	}.withType(&typeid(SayHiArg)));
	return true;
} //SayHiArg::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SayHiArg::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(SayHiArg))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			return std::make_unique<ValueWrap<String>>(name);
		case countID:
			return std::make_unique<ValueWrap<int32_t>>(count);
		case isHelloID:
			return std::make_unique<ValueWrap<bool>>(isHello);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SayHiArg::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void SayHiArg::setDefault() {
	name.clear();
	count = 0;
	isHello = false;
} //SayHiArg::setDefault
