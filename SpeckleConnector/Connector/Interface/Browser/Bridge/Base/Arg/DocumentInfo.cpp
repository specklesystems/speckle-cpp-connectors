#include "Connector/Interface/Browser/Bridge/Base/Arg/DocumentInfo.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		docLocate,
		docName,
		docID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"Location"},
		Identity{"Name"},
		Identity{"Id"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool DocumentInfo::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[docLocate], docLocate, element },
			{ fieldID[docName], docName, element },
			{ fieldID[docID], docID, element },
		},
	}.withType(&typeid(DocumentInfo)));
	return true;
} //DocumentInfo::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique DocumentInfo::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(DocumentInfo))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case docLocate:
			return std::make_unique<ValueWrap<String>>(location);
		case docName:
			return std::make_unique<ValueWrap<String>>(name);
		case docID:
			return std::make_unique<ValueWrap<String>>(ID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //DocumentInfo::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void DocumentInfo::setDefault() {
	location.clear();
	name.clear();
	ID.clear();
} //DocumentInfo::setDefault
