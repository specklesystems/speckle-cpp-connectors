#ifndef SPECKLE_INTERFACE_JS_BRIDGE_METHOD_BASE
#define SPECKLE_INTERFACE_JS_BRIDGE_METHOD_BASE

#include "Speckle/Utility/String.h"
#include "Speckle/Interface/Browser/Bridge/BridgeChild.h"

namespace speckle::interfac::browser::bridge {
	
	/*!
	 Interface for methods supporting a JS bridge
	*/
	class BridgeMethodBase : public BridgeChild {
	public:
		/*!
		 Register the method argument type for a specified bridge
		 @param bridge The target bridge name
		 @return A reference to this
		 */
		virtual BridgeMethodBase& registerArgument(const speckle::utility::String& bridge) = 0;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_METHOD_BASE
