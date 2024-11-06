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
#include "Connector/Interface/Browser/Bridge/Base/OpenUrl.h"
#include "Speckle/Event/Type/ProjectEvent.h"

#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Element/Element.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/Filter/SendFilter.h"
#include "Connector/Database/ModelCardDatabase.h"

using namespace connector::interfac::browser::bridge;

namespace {
#ifdef ARCHICAD
	void subscribeAllElementsToElementChangeEvents()
	{
		auto project = connector::connector()->getActiveProject().lock();
		if (!project)
			return;

		auto elementDatabase = project->getElementDatabase();
		//auto table = elementDatabase->getTables(speckle::database::ElementStorage::TableType::primary2D);
		//auto allElements = elementDatabase->findElements(nullptr, *table.begin());
		auto allElements = elementDatabase->findElements();

		for (const auto& id : allElements)
			ACAPI_Element_AttachObserver(id);

		int  i = 0;
	}
#endif
}

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
	addMethod<OpenUrl>();

	// POC: Attaching Observer to all elements is too slow, registration is commented out for now
	// subscribeAllElementsToElementChangeEvents();

} //BaseBridge::BaseBridge

/*--------------------------------------------------------------------
	Handle a project event

	event: The project event

	return: True if the event should be closed
 --------------------------------------------------------------------*/
bool BaseBridge::handle(const speckle::event::ProjectEvent& event) {
	using enum speckle::event::ProjectEvent::Type;
	switch (event.getType()) {
		case open: {
			sendEvent("documentChanged");
			// POC: Attaching Observer to all elements is too slow, registration is commented out for now
			// subscribeAllElementsToElementChangeEvents();
		} break;
		default:
			break;
	}
	return false;
} //BaseBridge::handle
