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
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GetConfig* clonePtr() const override { return new GetConfig{*this}; }

		// MARK: - Functions (const)
		
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		std::unique_ptr<active::serialise::Cargo> getArgument() const override;
		/*!
		 Get the configuration settings
		 @return The settings
		 */
		std::unique_ptr<active::serialise::Cargo> run(void) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG
