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
	Handle the menu selection
 
	event: The menu event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ConnectorMenu::handle(const MenuEvent& event) {
	app()->publish(Event{toggleConnectorPaletteID});
	return true;
} //ConnectorMenu::handle
