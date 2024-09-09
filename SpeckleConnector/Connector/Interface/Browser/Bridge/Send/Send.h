#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND
#define CONNECTOR_INTERFACE_BRIDGE_SEND

#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
		///Argument for a JS call to update the configuration
	using UpdateArgs = speckle::interfac::browser::bridge::JSArgType<speckle::utility::String>;

	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class Send : public speckle::interfac::browser::bridge::BridgeMethod<UpdateArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		Send();

		// MARK: - Functions (const)
		
		/*!
		 Send a specified model
		 @param modelCardID The ID of the madel to send
		 */
		void run(const speckle::utility::String& modelCardID) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND
