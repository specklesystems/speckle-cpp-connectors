#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_SEND_SETTINGS
#define CONNECTOR_INTERFACE_BRIDGE_GET_SEND_SETTINGS

#include "Active/Serialise/CargoHold.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 JS Function class to retrieve the send filters
	*/
	class GetSendSettings : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetSendSettings();

		// MARK: - Functions (const)
		
		/*!
		 Get the send filters
		 @return The send filters
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_SEND_SETTINGS
