#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_SEND_FILTERS
#define CONNECTOR_INTERFACE_BRIDGE_GET_SEND_FILTERS

#include "Active/Serialise/CargoHold.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 JS Function class to retrieve the send filters
	*/
	class GetSendFilters : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		GetSendFilters();

		// MARK: - Functions (const)
		
		/*!
		 Get the send filters
		 @return The send filters
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_SEND_FILTERS
