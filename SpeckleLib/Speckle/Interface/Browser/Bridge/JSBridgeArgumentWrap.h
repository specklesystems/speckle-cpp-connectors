#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeArgument.h"

namespace speckle::interface::browser::bridge {
	
	class JSBridgeArgument;
	
	/*!
	 Wrapper for bridge function arguments, determing the target requirement on demand
	*/
	class JSBridgeArgumentWrap : public active::serialise::Package {
	public:
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		JSBridgeArgumentWrap();
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
		 Get the JS object name
		 @return The JS object name (empty on failure)
		 */
		speckle::utility::String getObjectName() const { return m_argument ? m_argument->getObjectName() : speckle::utility::String{}; }
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
		
	private:
			///The function arguments
		std::shared_ptr<JSBridgeArgument> m_argument;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_WRAP
