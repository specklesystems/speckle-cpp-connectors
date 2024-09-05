#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetConfig : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		GetConfig();

		// MARK: - Functions (const)
		
		/*!
		 Get the configuration settings
		 @return The settings
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG
