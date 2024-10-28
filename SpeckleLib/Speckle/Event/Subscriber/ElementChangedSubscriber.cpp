#include "Speckle/Event/Subscriber/ElementChangedSubscriber.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Database/Storage/ArchicadDBase/Element/ArchicadElementDBaseEngine.h"
#include "Speckle/Event/Type/ElementChangedEvent.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace active::environment;
using namespace active::event;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::event;

namespace {
	
#ifdef ARCHICAD
	/*!
	 Callback for an Archicad element change
	 @param elemType Info about the last changed element
	 */
	GSErrCode __ACENV_CALL elementChangedCallback(const API_NotifyElementType* elemType) {
		if (addon() == nullptr)
			return NoError;

		switch (elemType->notifID)
		{
			case APINotifyElement_New: {
				ACAPI_Element_AttachObserver(elemType->elemHead.guid);
			} break;
			case APINotifyElement_Change:
			case APINotifyElement_Edit: {
				// TODO add IDs to temp List
			} break;
			case APINotifyElement_BeginEvents:
				// TODO clear the temp List
				break;
			case APINotifyElement_EndEvents: {
				// TODO send the event
			} break;
			default:
				break;
		}

		return NoError;
	}
#endif

}

/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription ElementChangedSubscriber::subscription() const {
	return { {ElementChangedEvent::ID} };
} //ElementChangedSubscriber::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ElementChangedSubscriber::receive(const Event& event) {
		//Pass a menu event to the specified handler function
	if (auto changeEvent = dynamic_cast<const ElementChangedEvent*>(&event); changeEvent != nullptr)
		return handle(*changeEvent);
	return false;
} //ElementChangedSubscriber::receive


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool ElementChangedSubscriber::start() {
#ifdef ARCHICAD
	GSErrCode err = ACAPI_Element_InstallElementObserver(elementChangedCallback);
	if (err != NoError)
		return false;

	return (ACAPI_Element_CatchNewElement(nullptr, elementChangedCallback) == NoError);
#else
	return false;
#endif
} //ElementChangedSubscriber::start


/*--------------------------------------------------------------------
	Stop participation (release resources etc)
  --------------------------------------------------------------------*/
void ElementChangedSubscriber::stop() {
#ifdef ARCHICAD
	ACAPI_Notification_CatchSelectionChange(nullptr);
#endif
} //ElementChangedSubscriber::stop
