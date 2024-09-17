#include "Speckle/Event/Subscriber/DocStoreSubscriber.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Event/Type/DocStoreMergeEvent.h"

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
	 Callback for an Archicad document merge operation
	 @param sourceObjects The source document objects to merge
	 */
	GSErrCode __ACENV_CALL docMergeCallback(const GS::Array<API_AddonObject>& sourceObjects) {
		if (addon() != nullptr)
			addon()->publishExternal(DocStoreMergeEvent{sourceObjects});
		return NoError;
	}
#endif

}

	//True if a doc merge subscriber has already started (only one is required - there are no variants)
bool speckle::event::DocStoreSubscriber::m_isStarted = false;

/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription DocStoreSubscriber::subscription() const {
	return { {DocStoreMergeEvent::ID} };
} //DocStoreSubscriber::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool DocStoreSubscriber::receive(const Event& event) {
		//Pass a menu event to the specified handler function
	if (auto docEvent = dynamic_cast<const DocStoreMergeEvent*>(&event); docEvent != nullptr)
		return handle(*docEvent);
	return false;
} //DocStoreSubscriber::receive


/*--------------------------------------------------------------------
	Attach participant components to the app (as required)
 
	return: True if the participant is able to function
  --------------------------------------------------------------------*/
bool DocStoreSubscriber::attach() {
#ifdef ARCHICAD
	ACAPI_AddOnObject_RegisterAddOnObjectHandler();
#endif
	return true;
} //DocStoreSubscriber::attach


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool DocStoreSubscriber::start() {
	if (m_isStarted)
		return true;
	m_isStarted = true;
#ifdef ARCHICAD
	return (ACAPI_AddOnObject_InstallAddOnObjectMergeHandler(docMergeCallback) == NoError);
#else
	return false;
#endif
} //DocStoreSubscriber::start
