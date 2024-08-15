#ifndef SPECKLE_INTERFACE_BRIDGE_RUN_METHOD
#define SPECKLE_INTERFACE_BRIDGE_RUN_METHOD

#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Interface/Browser/JSFunction.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeArgumentWrap.h"

namespace speckle::interface::browser::bridge {
	
	class BrowserBridge;
		
	/*!
	 Function to retrieve the names of the methods supported by the bridge
	*/
	class RunMethod : public JSFunction<JSBridgeArgumentWrap, void, PlatformBinding> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		RunMethod(BrowserBridge& bridge);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		RunMethod(const RunMethod& source) = default;
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		RunMethod* clonePtr() const override { return new RunMethod{*this}; }
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		std::unique_ptr<active::serialise::Cargo> getArgument() const override;
		
	private:
		/*!
		 Run a specified bridge method
		 @param argument The method arguments
		 */
		void runMethod(JSBridgeArgumentWrap& argument) const;

			///The parent browser bridge
		BrowserBridge& m_bridge;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_RUN_METHOD
