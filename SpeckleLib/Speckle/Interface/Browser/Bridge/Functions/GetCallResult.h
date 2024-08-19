#ifndef SPECKLE_INTERFACE_BRIDGE_GET_CALL_RESULT
#define SPECKLE_INTERFACE_BRIDGE_GET_CALL_RESULT

#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeArgumentWrap.h"

namespace speckle::interfac::browser::bridge {
	
	class BrowserBridge;
		
	/*!
	 Function to retrieve the names of the methods supported by the bridge
	*/
	class GetCallResult : public JSFunction<JSBridgeArgumentWrap, active::serialise::Cargo, PlatformBinding> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetCallResult(BrowserBridge& bridge);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		GetCallResult(const GetCallResult& source) = default;
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GetCallResult* clonePtr() const override { return new GetCallResult{*this}; }
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		std::unique_ptr<active::serialise::Cargo> getArgument() const override;
		
	private:
		/*!
		 Get the result for a specified call
		 @param argument The method arguments specifying the target bridge and requestID
		 @return The requested result (nullptr on failure)
		 */
		std::unique_ptr<active::serialise::Cargo> getResult(JSBridgeArgumentWrap& argument) const;

			///The parent browser bridge
		BrowserBridge& m_bridge;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_GET_CALL_RESULT
