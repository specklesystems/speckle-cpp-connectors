#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

#include "Active/Setting/ValueSetting.h"
#include "Active/Setting/Values/StringValue.h"
#include "Speckle/Interface/Browser/JSPortal.h"
#include "Speckle/Interface/Browser/Bridge/Functions/GetBindingsMethodNames.h"
#include "Speckle/Interface/Browser/Bridge/Functions/RunMethod.h"
#include "Speckle/Interface/Browser/Bridge/Functions/GetCallResult.h"

#include <algorithm>
#include <map>

using namespace active::serialise;
using namespace active::setting;
using namespace speckle::serialise::jsbase;
using namespace speckle::interface::browser;
using namespace speckle::interface::browser::bridge;
using namespace speckle::utility;

namespace speckle::interface::browser::bridge {
	
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
		//Populate the required browser bridge functions callable from JS
	emplace_back(std::make_unique<GetBindingsMethodNames>(*this));
	emplace_back(std::make_unique<RunMethod>(*this));
	emplace_back(std::make_unique<GetCallResult>(*this));
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
	Cache the result from a method function for the JS caller to retrieve
 
	result: The result cargo to send back to the JS
	requestID: The resquest ID from the JS caller (to correctly pair up the caller and result)
  --------------------------------------------------------------------*/
void BrowserBridge::cacheResult(std::unique_ptr<Cargo> result, String requestID) {
	if (m_portal == nullptr)
		throw;	//TODO: Add exception detail
	const std::lock_guard<std::mutex> lock(m_result->mutex);
	(*m_result)[requestID] = std::move(result);
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
