#ifndef CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG

#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
		///Arguments for a JS call to update the configuration
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
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		UpdateConfig* clonePtr() const override { return new UpdateConfig{*this}; }

		// MARK: - Functions (const)
		
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		std::unique_ptr<active::serialise::Cargo> getArgument() const override;
		/*!
		 Get the configuration settings
		 @return The new settings
		 */
		void run(const ConnectorConfig& config) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_UPDATE_CONFIG
