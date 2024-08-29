#include "Speckle/Interface/Browser/Bridge/BridgeArgumentWrap.h"

#include "Active/Serialise/Inventory/Inventory.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ValueSettingWrap.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Utility/BufferIn.h"
#include "Speckle/Utility/Exception.h"

using namespace active::serialise;
using namespace active::serialise::json;
using namespace active::setting;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::utility;

namespace speckle::interfac::browser::bridge {
	
		///Factory functions to construct arguments from linked bridge/method names
	std::unordered_map<String, BridgeArgumentWrap::Production> BridgeArgumentWrap::m_argumentFactory;

}

namespace {

	using enum active::serialise::Entry::Type;

		///The indices of the package items
	enum FieldIndex {
		args,
	};

	
		///The indices of the arguments array rows
	enum RowIndex {
		methodName,
		requestID,
		argsJSON,
	};
	

		///The package inventory
	auto myInventory = Inventory {
		{
			{ {"arg"}, args, 0, std::nullopt, true },	//The JS arguments are expressed as a flat array - use the array indices to map to expected vars
		},
	}.withType(&typeid(BridgeArgumentWrap));;
	
}

/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BridgeArgumentWrap::~BridgeArgumentWrap() {
} //BridgeArgumentWrap::~BridgeArgumentWrap


/*--------------------------------------------------------------------
	Fill an inventory with the cargo items
 
	inventory: The inventory to receive the cargo items
 
	return: True if items have been added to the inventory
  --------------------------------------------------------------------*/
bool BridgeArgumentWrap::fillInventory(Inventory& inventory) const {
	inventory.merge(myInventory);
	return true;
} //BridgeArgumentWrap::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique BridgeArgumentWrap::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(BridgeArgumentWrap))
		return nullptr;
	switch (item.index) {
		case FieldIndex::args: {
			switch (item.available - 1) {	//NB: Args are not labelled - in this instance we use the array row index to couple to an argument var
				case RowIndex::methodName:
					return std::make_unique<ValueWrap<String>>(m_methodName);
				case RowIndex::requestID:
					return std::make_unique<ValueWrap<String>>(m_requestID);
				case RowIndex::argsJSON:
					return std::make_unique<ValueWrap<String>>(m_argsJSON);
				default:
					return nullptr;
			}
		}
		default:
			return nullptr;	//Requested an unknown index
	}
} //BridgeArgumentWrap::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void BridgeArgumentWrap::setDefault() {
	m_methodName.clear();
	m_requestID.clear();
	m_argsJSON.clear();
	m_argument.reset();	//This will be populated once the target bridge and method are known (and hence the required argument type)
} //BridgeArgumentWrap::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool BridgeArgumentWrap::validate() {
		//Build an argument with the attributes obtained of none exists
	if (!m_argument)
		finaliseArgument();
		//Then ensure the argument object is valid
	return m_argument->validate();
} //BridgeArgumentWrap::validate


/*--------------------------------------------------------------------
	Make an argument object for a specified bridge method
 
	methodID: The name of the target method
	requestID: The ID of the request
	argument: The method argument data (serialised)
 
	return: An argument object (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<BridgeArgument> BridgeArgumentWrap::makeArgument(const String& methodID, const String& requestID, const String& argument) {
	if (auto maker = m_argumentFactory.find(methodID); (maker != m_argumentFactory.end())) {
		if (auto result = reinterpret_cast<BridgeArgument*>(maker->second(methodID, requestID)); result != nullptr) {
			try {
				//The argument is passed as an array of stringified JSON - first unpack the array
				JSONTransport transport;
				ValueSetting args;
				transport.receive(ValueSettingWrap{args}, Identity{}, argument);
				if (args.size() != result->parameterCount())
					throw Exception{"Function called with wrong number of parameters"};
				String unifiedArgument;
				if (dynamic_cast<Package*>(result) != nullptr) {
						//Unify the argument strings into a single JSON argument
					unifiedArgument = "{";
					int32_t argIndex = 0;
					bool isFirst = true;
					for (auto& arg : args.operator std::vector<active::utility::String>()) {
						if (isFirst)
							isFirst = false;
						else
							unifiedArgument += ",";
						unifiedArgument += "\"" + String{argIndex++} + "\":" + JSONTransport::convertFromJSONString(arg);
					}
					unifiedArgument += "}";
				} else {
						//An item can only receive a single parameter
					if (args.size() > 1)
						throw Exception{"Function called with wrong number of parameters"};
					unifiedArgument = args;
				}
					//And receive the unified argument into the method argument
				transport.receive(std::forward<Cargo&&>(*result), Identity{}, unifiedArgument);
				return std::unique_ptr<BridgeArgument>{result};
			} catch(std::runtime_error e) {
					//Populating the error cancels the method
				return std::make_unique<BridgeArgument>(methodID, requestID, String{e.what()});
			} catch(...) {
					//Populating the error cancels the method
				return std::make_unique<BridgeArgument>(methodID, requestID, String{"An unexpected error occurred parsing the method argument"});
			}
		}
	}
	return nullptr;
} //BridgeArgumentWrap::makeArgument


/*--------------------------------------------------------------------
	Finalise the output argument object based on the current object, method etc
  --------------------------------------------------------------------*/
void BridgeArgumentWrap::finaliseArgument() const {
		//Use the deserialised target bridge and method to establish the required arguments (if any)
	m_argument = BridgeArgumentWrap::makeArgument(m_methodName, m_requestID, m_argsJSON);
		//If the function doesn't take an argument, we still need to pass along the base class with object name, method etc
	if (!m_argument)
		m_argument = std::make_unique<BridgeArgument>(m_methodName, m_requestID);
} //BridgeArgumentWrap::finaliseArgument
