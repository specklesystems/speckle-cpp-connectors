#include "Connector/Interface/Browser/Bridge/Selection/SelectionChangeHandler.h"

#include "Active/Event/Event.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Event/ConnectorEventID.h"
#include "Speckle/Environment/Addon.h"

using namespace connector::interfac::browser::bridge;


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SelectionChangeHandler::SelectionChangeHandler() {
	int i = 0;
} //SelectionChangeHandler::SelectionChangeHandler

/*--------------------------------------------------------------------
	Handle the menu selection

	event: The menu event

	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool SelectionChangeHandler::handle(const speckle::event::SelectionEvent& event) {
	int i = 0;
	return true;
}
