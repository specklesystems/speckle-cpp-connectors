#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG

#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetConfig : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetConfig();

		// MARK: - Functions (const)
		
		/*!
		 Get the configuration settings
		 @return The settings
		 */
		std::unique_ptr<active::serialise::Cargo> run(void) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG
