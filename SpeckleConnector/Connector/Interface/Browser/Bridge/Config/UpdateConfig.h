#ifndef CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG

#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
		///Argument for a JS call to update the configuration
	using UpdateArgs = speckle::interfac::browser::bridge::JSArgType<ConnectorConfig>;

	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class UpdateConfig : public speckle::interfac::browser::bridge::JSBridgeMethod<UpdateArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
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
