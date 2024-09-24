#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND
#define CONNECTOR_INTERFACE_BRIDGE_SEND

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
		///Argument parameter for a string
	using StringHold = active::serialise::CargoHold<active::serialise::ValueWrap<speckle::utility::String>, speckle::utility::String>;
		///Argument type for this method
	using SendArgs = speckle::interfac::browser::bridge::JSArgType<StringHold>;

	/*!
	 JS Function class to send a specified model
	*/
	class Send : public speckle::interfac::browser::bridge::BridgeMethod<SendArgs, void> {
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
		 @param modelCardID The ID of the model to send
		 */
		void run(const speckle::utility::String& modelCardID) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND
