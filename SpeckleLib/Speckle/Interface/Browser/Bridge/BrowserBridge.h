#ifndef SPECKLE_INTERFACE_BROWSER_BRIDGE
#define SPECKLE_INTERFACE_BROWSER_BRIDGE

#include "Speckle/Interface/Browser/JSObject.h"
#include "Speckle/Interface/Browser/Functional.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethodBase.h"

namespace active::setting {
	class ValueSetting;
}

namespace speckle::interfac::browser::bridge {
	
	/*!
	 A class to provide JS bridging for Speckle connectors using an internal table of defined methods
	*/
	class BrowserBridge : public JSObject<> {
	public:
		
		// MARK: - Types
		
		using base = JSObject<>;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param name The JS object name
		 */
		BrowserBridge(const utility::String& name);
		/*!
		 Destructor
		 */
		virtual ~BrowserBridge();
		
		// MARK: - Functions (const)
		
		/*!
		 Get the names of the methods supported by this bridge
		 @return The supported method names
		 */
		std::vector<utility::String> getMethodNames() const;
		/*!
		 Get a browser method by name
		 @return A pointer to the requested method (owner does not take ownership, nullptr = failure)
		 */
		Functional<>* getMethod(const speckle::utility::String& name) const;
		/*!
		 Send a named event through the JS portal
		 @param eventName The result cargo to send back to the JS
		 @param data Optional data to send with the event (nullptr = send event without data)
		 */
		void sendEvent(speckle::utility::String eventName, std::unique_ptr<active::serialise::Cargo> data = nullptr);
		
		// MARK: - Functions (mutating)
		
		/*!
		 Cache the result from a method function for the JS caller to retrieve
		 @param result The result cargo to send back to the JS
		 @param requestID The resquest ID from the JS caller (to correctly pair up the caller and result)
		 @param isNotified True if notification of the result availability should be sent to the JS portal
		 */
		void cacheResult(std::unique_ptr<active::serialise::Cargo> result, speckle::utility::String requestID, bool isNotified = true);
		/*!
		 Release the results linked to a specified request ID
		 @param requestID The required result ID
		 @return The results linked to the specified ID (nullptr on failure)
		 */
		std::unique_ptr<active::serialise::Cargo> releaseResult(speckle::utility::String requestID);
		
	protected:
		/*!
		 Add a bridge function
		 @tparam T The function type
		 */
		template<typename T> requires (std::is_base_of_v<Functional<>, T>)
		void addFunction() {
				//The argument type is registered against the bridge to enable an appropriate object to be deserialised from the JS args
			auto function = std::make_shared<T>();
			if (auto child = dynamic_cast<BridgeChild*>(function.get()); child != nullptr)
				child->setBridge(this);
			emplace_back(function);
		}
		/*!
		 Add a browser method
		 @tparam T The method type (which also defines the argument type)
		 */
		template<typename T> requires (std::is_base_of_v<BridgeMethodBase, T>)
		void addMethod() {
				//The argument type is registered against the bridge to enable an appropriate object to be deserialised from the JS args
			auto method = std::make_shared<T>();
			method->registerArgument(getName());
			method->setBridge(this);
			m_methods->insert(m_methods->end(), method);
		}
		
	private:
			///List of methods supported by the bridge
		using BridgedMethods = std::vector<std::shared_ptr<Functional<>>>;
			///Methods supported by the bridge
		std::unique_ptr<BridgedMethods> m_methods = std::make_unique<BridgedMethods>();
			//Result cache
		class ResultCache;
		std::unique_ptr<ResultCache> m_result;
	};
	
}

#endif	//SPECKLE_INTERFACE_BROWSER_BRIDGE
