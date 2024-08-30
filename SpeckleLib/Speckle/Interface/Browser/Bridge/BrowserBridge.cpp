#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

#include "Active/Setting/ValueSetting.h"
#include "Active/Setting/Values/StringValue.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Interface/Browser/JSPortal.h"
#include "Speckle/Interface/Browser/Bridge/Functions/GetBindingsMethodNames.h"
#include "Speckle/Interface/Browser/Bridge/Functions/RunMethod.h"
#include "Speckle/Interface/Browser/Bridge/Functions/GetCallResult.h"
#include "Speckle/Utility/Guid64.h"

#include <algorithm>
#include <map>

using namespace active::serialise;
using namespace active::setting;
using namespace speckle::serialise::jsbase;
using namespace speckle::interfac::browser;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::utility;

namespace speckle::interfac::browser::bridge {
	
	class BrowserBridge::ResultCache : public std::map<String, std::unique_ptr<Cargo>> {
	public:
			//Mutex to control access to the cache
		std::mutex mutex;
	};

}

/*--------------------------------------------------------------------
	Constructor
 
	name: The JS object name
	toReserve: The number of supported methods to reserve space for
  --------------------------------------------------------------------*/
BrowserBridge::BrowserBridge(const String& name) : JSObject{name} {
	m_result = std::make_unique<ResultCache>();
		//Populate the required browser bridge functions callable from JS
	addFunction<GetBindingsMethodNames>();
	addFunction<RunMethod>();
	addFunction<GetCallResult>();
} //BrowserBridge::BrowserBridge


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BrowserBridge::~BrowserBridge() {
} //BrowserBridge::~BrowserBridge


/*--------------------------------------------------------------------
	Get the names of the methods supported by this bridge
 
	return: The supported method names
  --------------------------------------------------------------------*/
ValueSetting BrowserBridge::getMethodNames() const {
	ValueSetting result;
	for (const auto& method : *m_methods)
		result.emplace_back(StringValue{method->getName()});
	return result;
} //BrowserBridge::getMethodNames


/*--------------------------------------------------------------------
	Get a browser method by name
 
	return: A pointer to the requested method (owner does not take ownership, nullptr = failure)
  --------------------------------------------------------------------*/
Functional<>* BrowserBridge::getMethod(const String& name) const {
	Functional<>* result = nullptr;
	if (auto method = std::find_if(m_methods->begin(), m_methods->end(), [&](const auto& i) { return i->getName() == name; }); method != m_methods->end())
		result = method->get();
	return result;
} //BrowserBridge::getMethod


/*--------------------------------------------------------------------
	Send a named event through the JS portal
 
	eventName: The result cargo to send back to the JS
	data: Optional data to send with the event (nullptr = send event without data)
  --------------------------------------------------------------------*/
void BrowserBridge::sendEvent(speckle::utility::String eventName, std::unique_ptr<active::serialise::Cargo> data) {
	if (m_portal == nullptr)
		throw;	//TODO: Add exception detail
	if (!data) {
			//Just send an event with no data
		m_portal->execute(getName() + ".emit('" + eventName + "')");
		return;
	}
		//Generate a custom requestID for the data
	String requestID{Guid64{true} + String{"_"} + eventName};
	cacheResult(std::move(data), requestID, false);
		//Send a notification using the event name and request ID to signal that the data is available
	m_portal->execute(getName() + ".emitResponseReady('" + eventName + "','" + requestID + "')");
} //BrowserBridge::sendEvent


/*--------------------------------------------------------------------
	Cache the result from a method function for the JS caller to retrieve
 
	result: The result cargo to send back to the JS
	requestID: The resquest ID from the JS caller (to correctly pair up the caller and result)
	isNotified: True if notification of the result availability should be sent to the JS portal
  --------------------------------------------------------------------*/
void BrowserBridge::cacheResult(std::unique_ptr<Cargo> result, String requestID, bool isNotified) {
	if (m_portal == nullptr)
		throw;	//TODO: Add exception detail
	const std::lock_guard<std::mutex> lock(m_result->mutex);
	(*m_result)[requestID] = std::move(result);
		//Notify that the result is available when required
	if (isNotified)
		m_portal->execute(getName() + ".responseReady('" + requestID + "')");	//TODO: Need to confirm target object name
} //BrowserBridge::cacheResult


/*--------------------------------------------------------------------
	Release the results linked to a specified request ID
 
	requestID: The required result ID
 
	return: The results linked to the specified ID (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> BrowserBridge::releaseResult(String requestID) {
	std::unique_ptr<Cargo> result;
	if (auto iter = m_result->find(requestID); iter != m_result->end()) {
		result = std::move(iter->second);
		m_result->erase(iter);
	}
	return result;
} //BrowserBridge::releaseResult
