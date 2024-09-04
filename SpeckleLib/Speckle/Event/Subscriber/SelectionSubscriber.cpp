#include "Speckle/Event/Subscriber/SelectionSubscriber.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Event/Type/SelectionEvent.h"

using namespace active::environment;
using namespace active::event;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::event;

namespace {
	
#ifdef ARCHICAD
	/*!
	 Callback for an Archicad selection change
	 @param params Info about the last selected element
	 */
	GSErrCode __ACENV_CALL selectionCallback(const API_Neig* params) {
		if (addon() != nullptr) {
			auto selection = (params == nullptr) ? Link{} : Link{*params};
			addon()->publishExternal(SelectionEvent{selection});
		}
		return NoError;
	}
#endif

}

/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription SelectionSubscriber::subscription() const {
	return { {SelectionEvent::ID} };
} //SelectionSubscriber::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool SelectionSubscriber::receive(const Event& event) {
		//Pass a menu event to the specified handler function
	if (auto selectEvent = dynamic_cast<const SelectionEvent*>(&event); selectEvent != nullptr)
		return handle(*selectEvent);
	return false;
} //SelectionSubscriber::receive


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool SelectionSubscriber::start() {
#ifdef ARCHICAD
	return (ACAPI_Notification_CatchSelectionChange(selectionCallback) == NoError);
#else
	return false;
#endif
} //SelectionSubscriber::start
