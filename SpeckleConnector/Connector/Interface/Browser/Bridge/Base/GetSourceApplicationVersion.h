#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_APP_VERSION
#define CONNECTOR_INTERFACE_BRIDGE_GET_APP_VERSION

#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 JS Function class to get the host application version
	*/
	class GetSourceApplicationVersion : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetSourceApplicationVersion();

		// MARK: - Functions (const)
		
		/*!
		 Get the host application version
		 @return The application version
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_APP_VERSION
