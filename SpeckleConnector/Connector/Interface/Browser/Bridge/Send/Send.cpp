#include "Connector/Interface/Browser/Bridge/Send/Send.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendViaBrowserArgs.h"
#include "Speckle/Database/AccountDatabase.h"
#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Record/Credentials/Account.h"
#include "Speckle/Serialise/Detached/Storage/DetachedMemoryStore.h"
#include "Speckle/Utility/Exception.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
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
		//Collect targeted elements here
	SendObject toSend{std::make_unique<Record>()};	//NB: Using a placeholder object for now
	auto result = std::make_unique<SendViaBrowserArgs>(*modelCard, *account, std::move(toSend));
	getBridge()->sendEvent("sendByBrowser", std::move(result));	
} //Send::run
