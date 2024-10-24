#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_SEND_SETTINGS
#define CONNECTOR_INTERFACE_BRIDGE_GET_SEND_SETTINGS

#include "Active/Serialise/CargoHold.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 JS Function class to retrieve the send settings relevant to the host BIM application
	*/
	class GetSendSettings : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		GetSendSettings();

		// MARK: - Functions (const)
		
		/*!
		 Get send settings relevant to the host BIM application
		 @return The send settings
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_SEND_SETTINGS
