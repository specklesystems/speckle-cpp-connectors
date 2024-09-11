#include "Speckle/Event/Subscriber/ProjectSubscriber.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Event/Type/ProjectEvent.h"

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
	 Callback for an Archicad project event
	 @param notifyEvent The event notification type
	 @param param Parameter applicable to certain events (see API docs)
	 */
	GSErrCode __ACENV_CALL projectCallback(API_NotifyEventID notifyEvent, Int32 param) {
		if (addon() != nullptr)
			addon()->publishExternal(ProjectEvent{notifyEvent, param});
		return NoError;
	}
#endif

}

	//True if a doc merge subscriber has already started (only one is required - there are no variants)
bool speckle::event::ProjectSubscriber::m_isStarted = false;

/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription ProjectSubscriber::subscription() const {
	return { {ProjectEvent::ID} };
} //ProjectSubscriber::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ProjectSubscriber::receive(const Event& event) {
		//Pass a menu event to the specified handler function
	if (auto projectEvent = dynamic_cast<const ProjectEvent*>(&event); projectEvent != nullptr)
		return handle(*projectEvent);
	return false;
} //ProjectSubscriber::receive


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool ProjectSubscriber::start() {
	if (m_isStarted)
		return true;
	m_isStarted = true;
#ifdef ARCHICAD
	return (ACAPI_ProjectOperation_CatchProjectEvent(API_AllNotificationMask, projectCallback) == NoError);
#else
	return false;
#endif
} //ProjectSubscriber::start
