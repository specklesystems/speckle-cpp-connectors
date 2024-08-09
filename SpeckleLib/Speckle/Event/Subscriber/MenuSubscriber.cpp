#include "Speckle/Event/Subscriber/MenuSubscriber.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Event/Type/MenuEvent.h"

using namespace active::environment;
using namespace active::event;
using namespace speckle::environment;
using namespace speckle::event;

namespace {
	
#ifdef ARCHICAD
	/*!
	 Callback for Archicad menus
	 @param params The parameters describing a selected menu item - these are bundled into a published MenuEvent for MenuSubscribers
	 */
	GSErrCode __ACENV_CALL menuCallback(const API_MenuParams* params) {
		if (addon() != nullptr)
			addon()->publishExternal(MenuEvent{params->menuItemRef.menuResID, params->menuItemRef.itemIndex});
		return NoError;
	}
#endif

}

/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription MenuSubscriber::subscription() const {
	return { {MenuEvent::ID} };
} //MenuSubscriber::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool MenuSubscriber::receive(const Event& event) {
		//Pass a menu event to the specified handler function
	if (auto menuEvent = dynamic_cast<const MenuEvent*>(&event); menuEvent != nullptr)
		return handle(*menuEvent);
	return false;
} //MenuSubscriber::receive


/*--------------------------------------------------------------------
	Attach participant components to the addon (as required)
 
	return: True if the participant is able to function
  --------------------------------------------------------------------*/
bool MenuSubscriber::attach() {
#ifdef ARCHICAD
	return (ACAPI_MenuItem_RegisterMenu(m_resourceID, m_helpID, m_location, m_flags) == NoError);
#else
	return false;
#endif
} //MenuSubscriber::attach


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool MenuSubscriber::start() {
#ifdef ARCHICAD
	return (ACAPI_MenuItem_InstallMenuHandler(m_resourceID, menuCallback) == NoError);
#else
	return false;
#endif
} //MenuSubscriber::start
