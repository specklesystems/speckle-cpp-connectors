#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Interface/Browser/Bridge/ArgumentBase.h"
#include "Speckle/Utility/String.h"

namespace speckle::interfac::browser::bridge {
		
	/*!
	 Base class for the argments passed from JavaScript to a named C++ method in a Speckle bridging object
	 
	 NB: The BridgeArgumentWrap class will:
	 - Deserialise the essential attributes for determining the target method and arguments;
	 - Create the correct BridgeArgument subclass for the emthod/argument and populate it with the collected attributes
	 Therefore, there is no need for this class to handle any deserialisation, and subclasses should only handle the core arguments data
	*/
	class BridgeArgument : public active::serialise::Cargo {
	public:
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		BridgeArgument() {}
		/*!
		 Constructor
		 @param methodName The name of the method to receive the argument
		 @param requestID An ID to be paired with the method return value
		 @param errorMessage Optional error message - populate on failure (method will not be called in this case)

		 */
		BridgeArgument(const speckle::utility::String& methodName,
						 const speckle::utility::String& requestID,
						 const speckle::utility::String::Option& errorMessage = std::nullopt) :
				m_methodName{methodName}, m_requestID{requestID}, m_errorMessage{errorMessage} {}
		/*!
		 Destructor
		 */
		virtual ~BridgeArgument() {}
		
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
		 Get the number of parameters in the argument
		 @return The number of parameters
		 */  
		virtual uint32_t parameterCount() const { return 1;  }

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
	template<typename T, uint32_t Params = 1>
	class JSArgType : public BridgeArgument, public T {
	public:
		
		/*!
		 Constructor
		 @param methodName The name of the method to receive the argument
		 @param requestID An ID to be paired with the method return value
		 @param errorMessage Optional error message - populate on failure (method will not be called in this case)
		 */
		JSArgType(const speckle::utility::String& methodName,
				const speckle::utility::String& requestID,
				const speckle::utility::String::Option& errorMessage = std::nullopt) : BridgeArgument{methodName, requestID, errorMessage} {
				//Tag the argument object as a template where possible
			if (auto arg = dynamic_cast<ArgumentBase*>(this); arg != nullptr)
				arg->setArgumentTemplate(true);
		}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		JSArgType(const JSArgType& source) : BridgeArgument{source}, T{source} {}

        /*!
                    Get the number of parameters in the argument
                    @return The number of parameters
                    */
        uint32_t parameterCount() const override { return Params; }
    };
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT
