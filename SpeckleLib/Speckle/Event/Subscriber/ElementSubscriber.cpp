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
	
	using enum ElementEvent::Type;
	
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
				// POC: Put this back once we decide to use Object Tracking
				// ACAPI_Element_AttachObserver(elemType->elemHead.guid);
				addon()->publishExternal(ElementEvent{newElem, BIMRecordID{elemType->elemHead.guid}});
			} break;
			case APINotifyElement_Change: {
				addon()->publishExternal(ElementEvent{changeElem, BIMRecordID{elemType->elemHead.guid}});
			} break;
			case APINotifyElement_Edit: {
				addon()->publishExternal(ElementEvent{editElem, BIMRecordID{elemType->elemHead.guid}});
			} break;
			case APINotifyElement_Delete: {
				addon()->publishExternal(ElementEvent{deleteElem, BIMRecordID{ elemType->elemHead.guid}});
			} break;
			case APINotifyElement_BeginEvents:
				addon()->publishExternal(ElementEvent{begin});
				break;
			case APINotifyElement_EndEvents: {
				addon()->publishExternal(ElementEvent{end});
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
#ifdef ServerMainVers_2700
	auto err = ACAPI_Element_InstallElementObserver(elementChangedCallback);
#else
	auto err = ACAPI_Notify_InstallElementObserver(elementChangedCallback);
#endif
	if (err != NoError)
		return false;
#ifdef ServerMainVers_2700
	return (ACAPI_Element_CatchNewElement(nullptr, elementChangedCallback) == NoError);
#else
	return (ACAPI_Notify_CatchNewElement(nullptr, elementChangedCallback) == NoError);
#endif
#else
	return false;
#endif
} //ElementSubscriber::start
