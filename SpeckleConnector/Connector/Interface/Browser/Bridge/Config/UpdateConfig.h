#ifndef CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG

#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
		///Argument for a JS call to update the display configuration settings
	using UpdateArgs = speckle::interfac::browser::bridge::JSArgType<ConnectorConfig>;

	/*!
	 JS Function class to update the display configuration settings 
	*/
	class UpdateConfig : public speckle::interfac::browser::bridge::BridgeMethod<UpdateArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		UpdateConfig();

		// MARK: - Functions (const)
		
		/*!
		 Update the configuration settings
		 @param config The new settings
		 */
		void run(const ConnectorConfig& config) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG
