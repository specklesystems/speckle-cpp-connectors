#ifndef SPECKLE_INTERFACE_BRIDGE_GET_CALL_RESULT
#define SPECKLE_INTERFACE_BRIDGE_GET_CALL_RESULT

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"
#include "Speckle/Interface/Browser/Bridge/BridgeArgumentWrap.h"
#include "Speckle/Interface/Browser/Bridge/BridgeChild.h"

namespace speckle::interfac::browser::bridge {
	
		//GetCallResult receives a string as an argument and returns the response as JSON (in a String) - both use a String wrapper
	using WrappedResultArg = active::serialise::CargoHold<active::serialise::ValueWrap<speckle::utility::String>, speckle::utility::String>;

	class BrowserBridge;
		
	/*!
	 Function to retrieve the names of the methods supported by the bridge
	*/
	class GetCallResult : public JSFunction<WrappedResultArg, WrappedResultArg, PlatformBinding>, public BridgeChild {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetCallResult();
		
	private:
		/*!
		 Get the result for a specified call
		 @param argument The method arguments specifying the target bridge and requestID
		 @return The requested result (nullptr on failure)
		 */
		std::unique_ptr<WrappedResultArg> getResult(WrappedResultArg& argument) const;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_GET_CALL_RESULT
