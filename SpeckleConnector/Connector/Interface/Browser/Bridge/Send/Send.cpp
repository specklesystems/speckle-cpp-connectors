#include "Connector/Interface/Browser/Bridge/Send/Send.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendViaBrowserArgs.h"
#include "Connector/Record/Collection/ProjectCollection.h"
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

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::serialise;
using namespace speckle::utility;

namespace {
		
}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Send::Send() : BridgeMethod{"Send", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the model to send
  --------------------------------------------------------------------*/
void Send::run(const String& modelCardID) const {
		//Find the specified model card
	auto modelCardDatabase = connector()->getModelCardDatabase();
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
		//Get the active project
	auto project = connector()->getActiveProject().lock();
	if (!project) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, noProjectOpenID), modelCardID));
		return;
	}
		//Build a collection from the selected elements
	auto collection = std::make_unique<ProjectCollection>(project);
	auto elementDatabase = project->getElementDatabase();
	auto selected = elementDatabase->getSelection();
	if (selected.empty()) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, noSelectedModelItemsID), modelCardID));
		return;
	}
	for (const auto& link : selected) {
		if (auto element = elementDatabase->getElement(link); element)
			collection->addElement(*element);
	}
		//Send the collected information
	auto result = std::make_unique<SendViaBrowserArgs>(*modelCard, *account, SendObject{std::move(collection)});
	getBridge()->sendEvent("sendByBrowser", std::move(result));
} //Send::run
