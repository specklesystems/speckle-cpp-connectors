#include "Connector/Interface/ConnectorMenu.h"

#include "Active/Event/Event.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Event/ConnectorEventID.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Event/Type/MenuEvent.h"

using namespace active::environment;
using namespace active::event;
using namespace connector;
using namespace speckle::event;

namespace {
	
	auto connectorMenuItem = 1;
	
}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ConnectorMenu::ConnectorMenu()
#ifdef ARCHICAD
	: MenuSubscriber{speckleMenu, speckleMenuHelp, MenuCode_Palettes}	//Menu located in the Window/Palettes menu
#endif
{
} //ConnectorMenu::ConnectorMenu


/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription ConnectorMenu::subscription() const {
	auto result = MenuSubscriber::subscription();
	result.insert(setConnectorMenuCheckID);
	return result;
} //ConnectorMenu::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ConnectorMenu::receive(const active::event::Event& event) {
	if (event != setConnectorMenuCheckID)
		return MenuSubscriber::receive(event);
		//Get the menu checked state from the event and apply it to the menu
	if (auto menuState = event.findValue(menuCheckStateID); menuState != nullptr)
		setMenuChecked(connectorMenuItem, menuState->boolVal());
	return false;
} //ConnectorMenu::receive


/*--------------------------------------------------------------------
	Handle the menu selection
 
	event: The menu event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ConnectorMenu::handle(const MenuEvent& event) {
	app()->publish(Event{toggleConnectorPaletteID});
	return true;
} //ConnectorMenu::handle
