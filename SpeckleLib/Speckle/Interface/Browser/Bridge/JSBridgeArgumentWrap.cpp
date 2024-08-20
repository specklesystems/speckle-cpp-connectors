#include "Speckle/Interface/Browser/Bridge/JSBridgeArgumentWrap.h"

#include "Active/Serialise/Inventory/Inventory.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::utility;

namespace speckle::interfac::browser::bridge {
	
		///Factory functions to construct arguments from linked bridge/method names
	std::unordered_map<speckle::utility::String, JSBridgeArgumentWrap::Production> JSBridgeArgumentWrap::m_argumentFactory;

}

namespace {

	using enum active::serialise::Entry::Type;

		///The indices of the package items
	enum FieldIndex {
		args,
	};

	
		///The indices of the arguments array rows
	enum RowIndex {
		objectName,
		methodName,
		requestID,
	};
	

		///The package inventory
	auto myInventory = Inventory {
		{
			{ {"arg"}, args, array },	//The JS arguments are expressed as a flat array - use the array indices to map to expected vars
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
bool JSBridgeArgumentWrap::fillInventory(Inventory& inventory) const {
	inventory.merge(myInventory);
	return true;
} //JSBridgeArgumentWrap::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique JSBridgeArgumentWrap::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(JSBridgeArgumentWrap))
		return nullptr;
	switch (item.index) {
		case FieldIndex::args: {
			switch (item.available) {	//NB: Args are not labelled - in this instance we use the array row index to couple to an argument var
				case RowIndex::objectName:
					return std::make_unique<ValueWrap<String>>(m_objectName);
				case RowIndex::methodName:
					return std::make_unique<ValueWrap<String>>(m_methodName);
				case RowIndex::requestID:
					return std::make_unique<ValueWrap<String>>(m_requestID);
				default:
						//Once the argument attributes have been obtained (object, method etc) we need to ensure the argument object exists
					if (!m_argument)
						finaliseArgument();
					Inventory::Item childItem{item};
						//The index of the child item starts at zero, so deduct the indices already received by the wrapper
					childItem.available -= (RowIndex::requestID + 1);
					return m_argument->getCargo(childItem);
			}
		}
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
		//Only successful if we built an argument object and its content is valid
	return !m_argument && m_argument->validate();
} //JSBridgeArgumentWrap::validate


/*--------------------------------------------------------------------
	Finalise the output argument object based on the current object, method etc
  --------------------------------------------------------------------*/
void JSBridgeArgumentWrap::finaliseArgument() const {
		//Use the deserialised target bridge and method to establish the required arguments (if any)
	m_argument.reset(JSBridgeArgumentWrap::makeArgument(m_objectName, m_methodName, m_requestID));
		//If the function doesn't take an argument, we still need to pass along the base class with object name, method etc
	if (!m_argument)
		m_argument = std::make_unique<JSBridgeArgument>(m_objectName, m_methodName, m_requestID);
} //JSBridgeArgumentWrap::finaliseArgument
