#ifndef CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_VERSION
#define CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_VERSION

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 JS Function class to get the connector software version
	*/
	class GetConnectorVersion : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetConnectorVersion();

		// MARK: - Functions (const)
		
		/*!
		 Get the connector software version
		 @return The connector software version
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_VERSION
