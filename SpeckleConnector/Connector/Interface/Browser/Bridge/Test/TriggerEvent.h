#ifndef CONNECTOR_INTERFACE_BRIDGE_TRIGGER_EVENT
#define CONNECTOR_INTERFACE_BRIDGE_TRIGGER_EVENT

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/CargoHold.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	using StringHold = active::serialise::CargoHold<active::serialise::ValueWrap<speckle::utility::String>, speckle::utility::String>;
		///Argument for the parameters of a test message
	using TriggerEventWrapper = speckle::interfac::browser::bridge::JSArgType<StringHold>;

	/*!
	 JS Function class to emit specified events as directed by JS
	*/
	class TriggerEvent : public speckle::interfac::browser::bridge::BridgeMethod<TriggerEventWrapper, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		TriggerEvent();

		// MARK: - Functions (const)
		
		/*!
		 Trigger an event based on a specified name
		 @param eventName The event name
		 */
		void run(speckle::utility::String eventName) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_TRIGGER_EVENT
