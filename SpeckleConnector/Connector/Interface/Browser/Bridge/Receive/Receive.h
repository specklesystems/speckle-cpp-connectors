#ifndef CONNECTOR_INTERFACE_BRIDGE_RECEIVE
#define CONNECTOR_INTERFACE_BRIDGE_RECEIVE

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
		///Argument parameter for a string
	using StringHold = active::serialise::CargoHold<active::serialise::ValueWrap<speckle::utility::String>, speckle::utility::String>;
		///Argument type for this method
	using ReceiveArgs = speckle::interfac::browser::bridge::JSArgType<StringHold>;

	/*!
	 JS Function class to receive a specified model
	*/
	class Receive : public speckle::interfac::browser::bridge::BridgeMethod<ReceiveArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		Receive();

		// MARK: - Functions (const)
		
		/*!
		 Receive a specified model
		 @param modelCardID The ID of the model card identifying the objects to receive
		 */
		void run(const speckle::utility::String& modelCardID) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_RECEIVE
