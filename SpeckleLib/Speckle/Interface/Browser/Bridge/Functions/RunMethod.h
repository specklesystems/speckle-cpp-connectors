#ifndef SPECKLE_INTERFACE_BRIDGE_RUN_METHOD
#define SPECKLE_INTERFACE_BRIDGE_RUN_METHOD

#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"
#include "Speckle/Interface/Browser/Bridge/BridgeArgumentWrap.h"
#include "Speckle/Interface/Browser/Bridge/BridgeChild.h"

namespace speckle::interfac::browser::bridge {
	
	class BrowserBridge;
		
	/*!
	 Function to retrieve the names of the methods supported by the bridge
	*/
	class RunMethod : public JSFunction<BridgeArgumentWrap, void, PlatformBinding>, public BridgeChild {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		RunMethod();
		
	private:
		/*!
		 Run a specified bridge method
		 @param argument The method arguments
		 */
		void runMethod(BridgeArgumentWrap& argument) const;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_RUN_METHOD
