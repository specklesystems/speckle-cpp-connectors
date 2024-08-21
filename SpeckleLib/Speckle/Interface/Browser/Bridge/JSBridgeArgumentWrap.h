#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeArgument.h"

#include <unordered_map>

namespace speckle::interfac::browser::bridge {
	
	class JSBridgeArgument;

	/*!
		Factory function to make an argument object
		@param method The target method to receive the argument
		@param request The request ID for the result
		@return A new argument object
	*/
	template<typename T>
	void* constructArgument(const speckle::utility::String& method, const speckle::utility::String& request) {
		try {
			return new T(method, request);
		} catch(...) {
			return nullptr;	//Object constructors should throw an exception if incoming data isn't viable (NB: only use for unrecoverable problems)
		}
	}
	
	/*!
	 Wrapper for bridge function arguments, determing the target requirement on demand
	*/
	class JSBridgeArgumentWrap : public active::serialise::Package {
	public:
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		JSBridgeArgumentWrap() {}
		/*!
		 Copy constructor
		 */
		JSBridgeArgumentWrap(const JSBridgeArgumentWrap&) = default;
		/*!
		 Destructor
		 */
		~JSBridgeArgumentWrap();
		
		// MARK: - Operators

		/*!
		 Conversion operator
		 @return True if the arguments are populated
		 */
		operator bool() { return m_argument.operator bool(); }

		// MARK: - Functions (const)
		
		/*!
		 Get the bridge function arguments
		 @return The bridge arguments (nullptr on failure)
		 */
		std::shared_ptr<JSBridgeArgument> get() const { return m_argument; }
		/*!
		 Get the associated method name for the argument
		 @return The method name (empty on failure)
		 */
		speckle::utility::String getMethodName() const { return m_argument ? m_argument->getMethodName() : speckle::utility::String{}; }
		/*!
		 Get the request ID from the JS caller (to be paired with the method result)
		 @return The request ID (empty on failure)
		 */
		speckle::utility::String getRequestID() const { return m_argument ? m_argument->getRequestID() : speckle::utility::String{}; }
		/*!
		 Get the function argument (as a JSON string, expected to contain the argument values in an array)
		 @return The function argument (expressed as "[]" for functions that take no arguments)
		 */
		speckle::utility::String getArgJSON() const { return m_argsJSON; }

		// MARK: - Functions (serialisation)
		
		/*!
		 Fill an inventory with the cargo items
		 @param inventory The inventory to receive the cargo items
		 @return True if items have been added to the inventory
		*/
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure)
		 */
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		/*!
		 Set to the default package content
		 */
		void setDefault() override;
		/*!
		 Validate the cargo data
		 @return True if the data has been validated
		 */
		bool validate() override;

		/*!
		 Make an argument object for a specified bridge method
		 @param method The name of the target method
		 @return An argument object (nullptr on failure)
		 */
		static JSBridgeArgument* makeArgument(const speckle::utility::String& method, const speckle::utility::String& request) {
			if (auto maker = m_argumentFactory.find(method); (maker != m_argumentFactory.end()))
				return reinterpret_cast<JSBridgeArgument*>(maker->second(method, request));
			return nullptr;
		}
		
		/*!
		 Add a factory method for constructing the arguments of a specified bridge method
		 @param method The name of the target method
		 */
		template<typename T> requires std::is_base_of_v<JSBridgeArgument, T>
		static void defineArgument(const speckle::utility::String& method) {
			m_argumentFactory[method] = constructArgument<T>;
		}
		
	private:
		/*!
		 Finalise the output argument object based on the current object, method etc
		 */
		void finaliseArgument() const;

			//Factory function for producing instances from a serialised document object
		using Production = std::function<void*(const speckle::utility::String&, const speckle::utility::String&)>;
			///Factory functions to construct arguments from linked bridge/method names
		static std::unordered_map<speckle::utility::String, Production> m_argumentFactory;
		
			///The name of the method to receive the argument
		speckle::utility::String m_methodName;
			///An ID to be paired with the method return value
		speckle::utility::String m_requestID;
			///the function arguments as JSON
		speckle::utility::String m_argsJSON;
			///The wrapped function arguments
		mutable std::shared_ptr<JSBridgeArgument> m_argument;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP
