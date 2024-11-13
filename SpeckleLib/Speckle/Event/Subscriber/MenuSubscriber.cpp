#include "Speckle/Event/Subscriber/MenuSubscriber.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Event/Type/MenuEvent.h"

#ifdef ARCHICAD
#ifdef ServerMainVers_2700
#include <ACAPI_Interface.h>
#endif
#endif

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
	Set whether a menu item is enabled
 
	item: The menu item index
	state: True if the menu item is enabled
  --------------------------------------------------------------------*/
void MenuSubscriber::setMenuEnabled(uint16_t item, bool state) const {
#ifdef ARCHICAD
	API_MenuItemRef menuRef{};
	menuRef.menuResID = m_resourceID;
	menuRef.itemIndex = item;
#ifdef ServerMainVers_2700
	GSFlags flags{};
	if (ACAPI_MenuItem_GetMenuItemFlags(&menuRef, &flags) == NoError)
#else
	Int32 flags = 0;
	if (ACAPI_Interface(APIIo_GetMenuItemFlagsID, &menuRef, &flags) == NoError)
#endif //ServerMainVers_2600
	{
		bool isActive = ((flags & API_MenuItemDisabled) == 0);
		if (isActive != state) {
			if (state)
				flags &= ~API_MenuItemDisabled;
			else
				flags |= API_MenuItemDisabled;
#ifdef ServerMainVers_2700
			ACAPI_MenuItem_SetMenuItemFlags(&menuRef, &flags);
#else
			ACAPI_Interface(APIIo_SetMenuItemFlagsID, &menuRef, &flags);
#endif //ServerMainVers_2600
		}
	}
#endif //ARCHICAD
} //MenuSubscriber::setMenuEnabled


/*--------------------------------------------------------------------
	Set whether a menu item is checked
 
	item: The menu item index
	state: True if the menu item is checked
  --------------------------------------------------------------------*/
void MenuSubscriber::setMenuChecked(uint16_t item, bool state) const {
#ifdef ARCHICAD
	API_MenuItemRef menuRef{};
	menuRef.menuResID = m_resourceID;
	menuRef.itemIndex = item;
#ifdef ServerMainVers_2700
	GSFlags flags{};
	if (ACAPI_MenuItem_GetMenuItemFlags(&menuRef, &flags) == NoError)
#else
	Int32 flags = 0;
	if (ACAPI_Interface(APIIo_GetMenuItemFlagsID, &menuRef, &flags) == NoError)
#endif //ServerMainVers_2600
	{
		bool isChecked = ((flags & API_MenuItemChecked) != 0);
		if (isChecked != state) {
			if (state)
				flags |= API_MenuItemChecked;
			else
				flags &= ~API_MenuItemChecked;
#ifdef ServerMainVers_2700
			ACAPI_MenuItem_SetMenuItemFlags(&menuRef, &flags);
#else
			ACAPI_Interface(APIIo_SetMenuItemFlagsID, &menuRef, &flags);
#endif //ServerMainVers_2600
		}
	}
#endif
} //MenuSubscriber::setMenuChecked


/*--------------------------------------------------------------------
	Set the label of a menu item
 
	item: The menu item index
	text: The menu item text
  --------------------------------------------------------------------*/
void MenuSubscriber::setMenuLabel(uint16_t item, const speckle::utility::String& text) const {
#ifdef ARCHICAD
	API_MenuItemRef menuRef{};
	menuRef.menuResID = m_resourceID;
	menuRef.itemIndex = item;
	GS::UniString label(text);
#ifdef ServerMainVers_2700
	ACAPI_MenuItem_SetMenuItemText(&menuRef, nullptr, &label);
#else
	ACAPI_Interface(APIIo_SetMenuItemTextID, &menuRef, nullptr, (void*) &label);
#endif
#endif
} //MenuSubscriber::setMenuLabel


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
#ifdef ServerMainVers_2700
	return (ACAPI_MenuItem_RegisterMenu(m_resourceID, m_helpID, m_location, m_flags) == NoError);
#else
	return (ACAPI_Register_Menu(m_resourceID, m_helpID, m_location, m_flags) == NoError);
#endif
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
#ifdef ServerMainVers_2700
	return (ACAPI_MenuItem_InstallMenuHandler(m_resourceID, menuCallback) == NoError);
#else
	return (ACAPI_Install_MenuHandler(m_resourceID, menuCallback) == NoError);
#endif
#else
	return false;
#endif
} //MenuSubscriber::start
