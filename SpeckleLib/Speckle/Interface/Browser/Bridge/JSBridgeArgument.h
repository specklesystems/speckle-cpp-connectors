#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace speckle::interfac::browser::bridge {
		
	/*!
	 Base class for the argments passed from JavaScript to a named C++ method in a Speckle bridging object
	 
	 NB: The JSBridgeArgumentWrap class will:
	 - Deserialise the essential attributes for determining the target method and arguments;
	 - Create the correct JSBridgeArgument subclass for the emthod/argument and populate it with the collected attributes
	 Therefore, there is no need for this class to handle any deserialisation, and subclasses should only handle the core arguments data
	*/
	class JSBridgeArgument : public active::serialise::Package {
	public:
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		JSBridgeArgument() {}
		/*!
		 Constructor
		 @param methodName The name of the method to receive the argument
		 @param requestID An ID to be paired with the method return value
		 @param errorMessage Optional error message - populate on failure (method will not be called in this case)

		 */
		JSBridgeArgument(const speckle::utility::String& methodName,
						 const speckle::utility::String& requestID,
						 const speckle::utility::String::Option& errorMessage = std::nullopt) :
				m_methodName{methodName}, m_requestID{requestID}, m_errorMessage{errorMessage} {}
		/*!
		 Destructor
		 */
		virtual ~JSBridgeArgument() {}
		
		// MARK: - Functions (const)
		
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
		/*!
		 Determine if the argument contains an error
		 @return True if the argument contains an error
		 */
		bool hasError() const { return m_errorMessage.operator bool(); }
		/*!
		 Get any error message relating to the arguments
		 @return The error message (nullopt if no errors occurred)
		 */
		speckle::utility::String::Option errorMessage() const { return m_errorMessage; }
		/*!
		 Fill an inventory with the cargo items
		 @param inventory The inventory to receive the cargo items
		 @return True if items have been added to the inventory
		 */
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure
		 */
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Set to the default package content
		 */
		void setDefault() override;
		
	private:
			///The name of the method to receive the argument
		speckle::utility::String m_methodName;
			///An ID to be paired with the method return value
		speckle::utility::String m_requestID;
			///Optional error message - only populated on failure to obtain a valid argument (method will not be called in this case)
		speckle::utility::String::Option m_errorMessage;
	};
	
		///Definition of the argument for a JS callable method (enclosing the local function argument)
	template<typename T>
	class JSArgType : public JSBridgeArgument {
	public:
		using JSBridgeArgument::JSBridgeArgument;

		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		JSArgType(const JSArgType& source) : JSBridgeArgument{ source }, value{ source.value } {}

		T value;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT
