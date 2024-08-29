#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Interface/Browser/Bridge/BridgeArgument.h"

#include <unordered_map>

namespace speckle::interfac::browser::bridge {
	
	class BridgeArgument;

	/*!
		Factory function to make an argument object
		@param method The target method to receive the argument
		@param request The request ID for the result
		@return A new argument object
	*/
	template<typename T>
	void* constructArgument(const speckle::utility::String& method, const speckle::utility::String& request) {
		try {
			auto result = new T(method, request);
				//Tag the argument object as a template where possible
			if (auto arg = dynamic_cast<ArgumentBase*>(result); arg != nullptr)
				arg->setArgumentTemplate(true);
			return result;
		} catch(...) {
			return nullptr;	//Object constructors should throw an exception if incoming data isn't viable (NB: only use for unrecoverable problems)
		}
	}
	
	/*!
	 Wrapper for bridge function arguments, determing the target requirement on demand
	*/
	class BridgeArgumentWrap : public active::serialise::Package {
	public:
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		BridgeArgumentWrap() {}
		/*!
		 Copy constructor
		 */
		BridgeArgumentWrap(const BridgeArgumentWrap&) = default;
		/*!
		 Destructor
		 */
		~BridgeArgumentWrap();
		
		// MARK: - Operators

		/*!
		 Conversion operator
		 @return True if the argument is populated
		 */
		operator bool() { return m_argument.operator bool(); }

		// MARK: - Functions (const)
		
		/*!
		 Get the bridge function argument
		 @return The bridge argument (nullptr on failure)
		 */
		std::shared_ptr<BridgeArgument> get() const { return m_argument; }
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
		 @param methodID The name of the target method
		 @param requestID The ID of the request
		 @param argument The method argument data (serialised)
		 @return An argument object (nullptr on failure)
		 */
		static std::unique_ptr<BridgeArgument> makeArgument(const speckle::utility::String& methodID,
															  const speckle::utility::String& requestID,
															  const speckle::utility::String& argument);
		
		/*!
		 Add a factory method for constructing the arguments of a specified bridge method
		 @param method The name of the target method
		 */
		template<typename T> requires std::is_base_of_v<BridgeArgument, T>
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
			///The function arguments as JSON
		speckle::utility::String m_argsJSON;
			///The wrapped function arguments
		mutable std::shared_ptr<BridgeArgument> m_argument;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP
