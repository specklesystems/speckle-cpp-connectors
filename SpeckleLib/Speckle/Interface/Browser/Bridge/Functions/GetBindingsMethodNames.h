#ifndef SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES
#define SPECKLE_INTERFACE_BRIDGE_GET_METHOD_NAMES

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ValueSettingWrap.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"

namespace speckle::interface::browser::bridge {
	
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
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GetBindingsMethodNames* clonePtr() const override { return new GetBindingsMethodNames{*this}; }
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		std::unique_ptr<active::serialise::Cargo> getArgument() const override { return nullptr; }	//Method doesn't take any arguments
		
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
