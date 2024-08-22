#ifndef SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES
#define SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ValueSettingWrap.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"

namespace speckle::interfac::browser::bridge {
	
	class BrowserBridge;
	
	using WrappedValue = active::serialise::CargoHold<active::serialise::ValueSettingWrap, active::setting::ValueSetting>;
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetBindingsMethodNames : public JSFunction<void, WrappedValue, PlatformBinding> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetBindingsMethodNames(BrowserBridge& bridge);
		
	private:
		/*!
		 Get the names of the methods supported by the parent browser
		 @return The supported method names
		 */
		std::unique_ptr<WrappedValue> getMethodNames() const;

			///Methods supported by the bridge
		BrowserBridge& m_bridge;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES
