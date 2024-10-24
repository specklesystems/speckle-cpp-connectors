#include "Connector/Interface/Browser/Bridge/Base/BaseBridge.h"
#include "Connector/Interface/Browser/Bridge/Base/AddModel.h"
#include "Connector/Interface/Browser/Bridge/Base/GetConnectorVersion.h"
#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"
#include "Connector/Interface/Browser/Bridge/Base/GetDocumentState.h"
#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationName.h"
#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationVersion.h"
#include "Connector/Interface/Browser/Bridge/Base/RemoveModel.h"
#include "Connector/Interface/Browser/Bridge/Base/UpdateModel.h"
#include "Connector/Interface/Browser/Bridge/Base/HighlightModel.h"
#include "Speckle/Event/Type/ProjectEvent.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
BaseBridge::BaseBridge() : BrowserBridge{"baseBinding"} {
		//Add bridge methods
	addMethod<AddModel>();
	addMethod<GetConnectorVersion>();
	addMethod<GetDocumentInfo>();
	addMethod<GetDocumentState>();
	addMethod<GetSourceApplicationName>();
	addMethod<GetSourceApplicationVersion>();
	addMethod<RemoveModel>();
	addMethod<UpdateModel>();
	addMethod<HighlightModel>();
} //BaseBridge::BaseBridge

/*--------------------------------------------------------------------
	Handle a project event

	event: The project event

	return: True if the event should be closed
 --------------------------------------------------------------------*/
bool BaseBridge::handle(const speckle::event::ProjectEvent& event) {
	using enum speckle::event::ProjectEvent::Type;
	switch (event.getType()) {
		case open:
			sendEvent("documentChanged");
			break;
		default:
			break;
	}
	return false;
} //BaseBridge::handle
