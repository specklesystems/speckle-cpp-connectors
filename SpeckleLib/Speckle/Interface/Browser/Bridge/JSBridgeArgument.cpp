#include "Speckle/Interface/Browser/Bridge/JSBridgeArgument.h"

#include "Active/Serialise/Inventory/Inventory.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace speckle::interface::browser::bridge;
using namespace speckle::utility;

namespace {

	using enum active::serialise::Entry::Type;

		///The indices of the package items
	enum FieldIndex {
		objectName,
		methodName,
		requestID,
	};

		///The package inventory
	auto myInventory = Inventory {
		{
			{ {"binding_name"}, objectName, attribute },
			{ {"name"}, methodName, attribute },
			{ {"request_id"}, requestID, attribute },
		},
	}.withType(&typeid(JSBridgeArgument));;

}

/*--------------------------------------------------------------------
	Fill an inventory with the cargo items
 
	inventory: The inventory to receive the cargo items
 
	return: True if items have been added to the inventory
  --------------------------------------------------------------------*/
bool JSBridgeArgument::fillInventory(active::serialise::Inventory& inventory) const {
	inventory.merge(myInventory);
	return true;
} //JSBridgeArgument::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique JSBridgeArgument::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(JSBridgeArgument))
		return nullptr;
	switch (item.index) {
		case FieldIndex::objectName:
			return std::make_unique<ValueWrap<String>>(m_objectName);
		case FieldIndex::methodName:
			return std::make_unique<ValueWrap<String>>(m_methodName);
		case FieldIndex::requestID:
			return std::make_unique<ValueWrap<String>>(m_requestID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //JSBridgeArgument::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void JSBridgeArgument::setDefault() {
	m_objectName.clear();
	m_methodName.clear();
	m_requestID.clear();
} //JSBridgeArgument::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool JSBridgeArgument::validate() {
	return !m_objectName.empty() && !m_methodName.empty() && !m_requestID.empty();
} //JSBridgeArgument::validate
