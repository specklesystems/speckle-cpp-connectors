#include "Speckle/Interface/Browser/Bridge/JSBridgeArgumentWrap.h"

#include "Active/Serialise/Inventory/Inventory.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace speckle::interface::browser::bridge;
using namespace speckle::utility;

namespace speckle::interface::browser::bridge {
	
		///Factory functions to construct arguments from linked bridge/method names
	std::unordered_map<speckle::utility::String, JSBridgeArgumentWrap::Production> JSBridgeArgumentWrap::m_argumentFactory;

}

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
	}.withType(&typeid(JSBridgeArgumentWrap));;

}

/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
JSBridgeArgumentWrap::~JSBridgeArgumentWrap() {
} //JSBridgeArgumentWrap::~JSBridgeArgumentWrap


/*--------------------------------------------------------------------
	Fill an inventory with the cargo items
 
	inventory: The inventory to receive the cargo items
 
	return: True if items have been added to the inventory
  --------------------------------------------------------------------*/
bool JSBridgeArgumentWrap::fillInventory(active::serialise::Inventory& inventory) const {
	if (!m_isReadingAttributes.has_value() || *m_isReadingAttributes)
		inventory.merge(myInventory);
	if (m_argument)
		m_argument->fillInventory(inventory);
	return true;
} //JSBridgeArgumentWrap::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique JSBridgeArgumentWrap::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(JSBridgeArgumentWrap))
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
} //JSBridgeArgumentWrap::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void JSBridgeArgumentWrap::setDefault() {
	m_objectName.clear();
	m_methodName.clear();
	m_requestID.clear();
	m_argument.reset();	//This will be populated once the target bridge and method are known (and hence the required argument type)
} //JSBridgeArgumentWrap::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool JSBridgeArgumentWrap::validate() {
	return !m_objectName.empty() && !m_methodName.empty() && !m_requestID.empty() && (!m_argument | m_argument->validate());
} //JSBridgeArgumentWrap::validate


/*--------------------------------------------------------------------
	Finalise the package attributes (called when isAttributeFirst = true and attributes have been imported)
 
	return: True if the attributes have been successfully finalised (returning false will cause an exception to be thrown)
  --------------------------------------------------------------------*/
bool JSBridgeArgumentWrap::finaliseAttributes() {
	if (!m_isReadingAttributes.has_value() || !*m_isReadingAttributes ||m_objectName.empty() || m_methodName.empty())
		return false;
	m_isReadingAttributes = false;
		//Use the deserialised target bridge and method to establish the required arguments (if any)
	m_argument.reset(JSBridgeArgumentWrap::makeArgument(m_objectName, m_methodName));
		//If the function doesn't take an argument, we still need to pass along the base class with object name, method etc
	if (!m_argument)
		m_argument = std::make_unique<JSBridgeArgument>(m_objectName, m_methodName, m_requestID);
	return true;
} //JSBridgeArgumentWrap::finaliseAttributes
