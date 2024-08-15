#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace speckle::interface::browser::bridge {
		
	/*!
	 Base class for the argments passed from JavaScript to a named C++ method in a Speckle bridging object
	*/
	class JSBridgeArgument : public active::serialise::Package {
	public:
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param objectName The name of the JS object the argument is targeting
		 @param methodName The name of the method to receive the argument
		 @param requestID An ID to be paired with the method return value
		 */
		JSBridgeArgument(const speckle::utility::String& objectName, const speckle::utility::String& methodName,
						 const speckle::utility::String& requestID) : m_objectName{objectName}, m_methodName{methodName}, m_requestID{requestID} {}
		/*!
		 Destructor
		 */
		virtual ~JSBridgeArgument() {}
		
		// MARK: - Functions (const)
		
		/*!
		 Get the JS object name
		 @return The JS object name
		 */
		const speckle::utility::String& getObjectName() const { return m_objectName; }
		/*!
		 Get the name of the method to receive the arguments
		 @return The method name
		 */
		const speckle::utility::String& getMethodName() const { return m_methodName; }
		/*!
		 Get the request ID from the JS caller (to be paired with the method result)
		 @return The request ID
		 */
		const speckle::utility::String& getRequestID() const { return m_requestID; }
		
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
			///The name of the JS object the argument is targeting
		speckle::utility::String m_objectName;
			///The name of the method to receive the argument
		speckle::utility::String m_methodName;
			///An ID to be paired with the method return value
		speckle::utility::String m_requestID;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT
