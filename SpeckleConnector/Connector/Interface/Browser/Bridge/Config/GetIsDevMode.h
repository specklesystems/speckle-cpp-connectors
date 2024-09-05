#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_IS_DEV_MODE
#define CONNECTOR_INTERFACE_BRIDGE_GET_IS_DEV_MODE

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve IsDebugMode
	*/
	class GetIsDevMode : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		GetIsDevMode();

		// MARK: - Functions (const)
		
		/*!
		 Get the DEBUG mode
		 @return true for DEBUG, false otherwise
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_IS_DEV_MODE
