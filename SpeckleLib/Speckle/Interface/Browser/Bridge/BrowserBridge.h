#ifndef SPECKLE_INTERFACE_BROWSER_BRIDGE
#define SPECKLE_INTERFACE_BROWSER_BRIDGE

#include "Speckle/Interface/Browser/JSObject.h"
#include "Speckle/Interface/Browser/Functional.h"

namespace active::setting {
	class ValueSetting;
}

namespace speckle::interface::browser::bridge {
	
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
		BrowserBridge(const speckle::utility::String& name);
		/*!
		 Destructor
		 */
		virtual ~BrowserBridge();
		
		// MARK: - Functions (const)
		
		/*!
		 Get the names of the methods supported by this bridge
		 @return The supported method names
		 */
		active::setting::ValueSetting getMethodNames() const;
		/*!
		 Get a browser method by name
		 @return A pointer to the requested method (owner does not take ownership, nullptr = failure)
		 */
		Functional<>* getMethod(const speckle::utility::String& name) const;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Cache the result from a method function for the JS caller to retrieve
		 @param result The result cargo to send back to the JS
		 @param requestID The resquest ID from the JS caller (to correctly pair up the caller and result)
		 */
		void cacheResult(std::unique_ptr<active::serialise::Cargo> result, speckle::utility::String requestID);
		/*!
		 Release the results linked to a specified request ID
		 @param requestID The required result ID
		 @return The results linked to the specified ID (nullptr on failure)
		 */
		std::unique_ptr<active::serialise::Cargo> releaseResult(speckle::utility::String requestID);
		
	protected:
		/*!
		 Add a browser method
		 */
		template<typename T> requires (std::is_base_of_v<Functional<>, T>)
		void addMethod() const { m_methods->emplace(T{}); }
		
	private:
			///List of methods supported by the bridge
		using BridgedMethods = active::container::Vector<Functional<>>;
			///Methods supported by the bridge
		std::unique_ptr<BridgedMethods> m_methods = std::make_unique<BridgedMethods>();
			//Result cache
		class ResultCache;
		std::unique_ptr<ResultCache> m_result;
	};
	
}

#endif	//SPECKLE_INTERFACE_BROWSER_BRIDGE
