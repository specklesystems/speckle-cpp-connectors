#ifndef SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES
#define SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ValueSettingWrap.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"
#include "Speckle/Interface/Browser/Bridge/BridgeChild.h"

namespace speckle::interfac::browser::bridge {
	
	class BrowserBridge;
	
	using WrappedValue = active::serialise::CargoHold<active::serialise::ValueSettingWrap, active::setting::ValueSetting>;
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetBindingsMethodNames : public JSFunction<void, WrappedValue, PlatformBinding>, public BridgeChild {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetBindingsMethodNames();
		
	private:
		/*!
		 Get the names of the methods supported by the parent browser
		 @return The supported method names
		 */
		std::unique_ptr<WrappedValue> getMethodNames() const;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES
