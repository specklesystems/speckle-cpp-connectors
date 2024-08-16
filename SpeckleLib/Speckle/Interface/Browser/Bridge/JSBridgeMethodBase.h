#ifndef SPECKLE_INTERFACE_JS_BRIDGE_METHOD_BASE
#define SPECKLE_INTERFACE_JS_BRIDGE_METHOD_BASE

#include "Speckle/Utility/String.h"

namespace speckle::interface::browser::bridge {
	
	/*!
	 Interface for methods supporting a JS bridge
	*/
	class JSBridgeMethodBase {
	public:
		/*!
		 Register the method argument type for a specified bridge
		 @param bridge The target bridge
		 @return A reference to this
		 */
		virtual JSBridgeMethodBase& registerArgument(const speckle::utility::String& bridge) const = 0;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_METHOD_BASE
