#include "Connector/Interface/Browser/Bridge/Send/Send.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendViaBrowserArgs.h"
#include "Connector/Record/Collection/ProjectCollection.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/Filter/SendFilter.h"
#include "Speckle/Database/AccountDatabase.h"
#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Record/Credentials/Account.h"
#include "Speckle/Serialise/Detached/Storage/DetachedMemoryStore.h"
#include "Speckle/Utility/Exception.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Element/Element.h"

using namespace speckle::record::element;
using namespace active::serialise;
using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::serialise;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Send::Send() : BridgeMethod{"Send", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the model card identifying the objects to send
  --------------------------------------------------------------------*/
void Send::run(const String& modelCardID) const {

#ifdef ARCHICAD
	// we currently rely on the ModelerAPI instead of the ModelAccessAPI
	// we have to open a 3D window to get the Mesh data of the model elements
	ACAPI_View_ShowAllIn3D();
#endif

		//Get the active project
	auto project = connector()->getActiveProject().lock();
	if (!project) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, noProjectOpenID), modelCardID));
		return;
	}
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return;
		//Find the specified model card
	auto modelCardDatabase = connectorProject->getModelCardDatabase();
	auto modelCard = modelCardDatabase->getCard(modelCardID);
	if (!modelCard) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, modelCardNotFoundID), modelCardID));
		return;
	}
		//Get the user account
	auto accountDatabase = connector()->getAccountDatabase();
	auto account = accountDatabase->getAccount(modelCard->getAccountID(), modelCard->getServerURL());
	if (!account) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, accountNotFoundID), modelCardID));
		return;
	}
		//Get the selected elements from the modelcard
	auto elementDatabase = project->getElementDatabase();
	ElementIDList selected{};
	if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
		selected = senderCard->getFilter().getElementIDs();
	}

		//Build a collection from the selected elements
	auto collection = std::make_unique<ProjectCollection>(project);
	for (const auto& link : selected) {
		if (auto element = elementDatabase->getElement(link); element)
			collection->addElement(*element);
	}
		//Send the collected information
	auto result = std::make_unique<SendViaBrowserArgs>(*modelCard, *account, SendObject{std::move(collection)});
	getBridge()->sendEvent("sendByBrowser", std::move(result));
} //Send::run
