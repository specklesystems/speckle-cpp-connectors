#ifndef CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_VERSION
#define CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_VERSION

#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetConnectorVersion : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetConnectorVersion();

		// MARK: - Functions (const)
		
		/*!
		 Get the host application version
		 @return The application version
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_VERSION
