#include "Speckle/Event/Subscriber/ElementSubscriber.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Database/Storage/ArchicadDBase/Element/ArchicadElementDBaseEngine.h"
#include "Speckle/Event/Type/ElementEvent.h"

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

		// POC: can we do this switch smater?
		switch (elemType->notifID)
		{
			case APINotifyElement_New: {
				ACAPI_Element_AttachObserver(elemType->elemHead.guid);
				addon()->publishExternal(ElementEvent{ ElementID{ elemType->elemHead.guid }, ElementEvent::EventType::New });
			} break;
			case APINotifyElement_Change: {
				addon()->publishExternal(ElementEvent{ ElementID{ elemType->elemHead.guid }, ElementEvent::EventType::Change });
			} break;
			case APINotifyElement_Edit: {
				addon()->publishExternal(ElementEvent{ ElementID{ elemType->elemHead.guid }, ElementEvent::EventType::Edit });
			} break;
			case APINotifyElement_Delete: {
				addon()->publishExternal(ElementEvent{ ElementID{ elemType->elemHead.guid }, ElementEvent::EventType::Delete });
			} break;
			case APINotifyElement_BeginEvents:
				addon()->publishExternal(ElementEvent{ ElementID{}, ElementEvent::EventType::Begin });
				break;
			case APINotifyElement_EndEvents: {
				addon()->publishExternal(ElementEvent{ ElementID{}, ElementEvent::EventType::End });
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
Subscriber::Subscription ElementSubscriber::subscription() const {
	return { {ElementEvent::ID} };
} //ElementSubscriber::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ElementSubscriber::receive(const Event& event) {
		//Pass a menu event to the specified handler function
	if (auto changeEvent = dynamic_cast<const ElementEvent*>(&event); changeEvent != nullptr)
		return handle(*changeEvent);
	return false;
} //ElementSubscriber::receive


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool ElementSubscriber::start() {
#ifdef ARCHICAD
	GSErrCode err = ACAPI_Element_InstallElementObserver(elementChangedCallback);
	if (err != NoError)
		return false;
	
	return (ACAPI_Element_CatchNewElement(nullptr, elementChangedCallback) == NoError);
#else
	return false;
#endif
} //ElementSubscriber::start


/*--------------------------------------------------------------------
	Stop participation (release resources etc)
  --------------------------------------------------------------------*/
void ElementSubscriber::stop() {
#ifdef ARCHICAD
	ACAPI_Notification_CatchSelectionChange(nullptr);
#endif
} //ElementSubscriber::stop
