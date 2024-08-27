#include "Speckle/Interface/Browser/Bridge/Functions/ErrorReport.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		errMessage,
		errInfo,
		errTrace,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"Message"},
		Identity{"Error"},
		Identity{"StackTrace"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ErrorReport::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[errMessage], errMessage, element },
			{ fieldID[errInfo], errInfo, element },
			{ fieldID[errTrace], errTrace, element },
		},
	}.withType(&typeid(ErrorReport)));
	return true;
} //ErrorReport::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ErrorReport::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ErrorReport))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case errMessage:
			return std::make_unique<ValueWrap<String>>(message);
		case errInfo:
			return std::make_unique<ValueWrap<String>>(error);
		case errTrace:
			return std::make_unique<ValueWrap<String>>(stackTrace);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ErrorReport::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ErrorReport::setDefault() {
	message.clear();
	error.clear();
	stackTrace.clear();
} //ErrorReport::setDefault
