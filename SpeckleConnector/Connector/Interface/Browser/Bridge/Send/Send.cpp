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

#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Element/Element.h"
using namespace speckle::record::element;

#include <array>

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
		//Collect targeted elements here

	
		//NB: This is a testing placeholder
	
	Element::Unique element;
	if (auto project = connector::connector()->getActiveProject().lock(); project) {
		auto elementDatabase = project->getElementDatabase();
		auto selected = elementDatabase->getSelection();
		for (const auto& link : selected)
			if (element = elementDatabase->getElement(link); element)
				break;
	}
	if (!element) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, noSelectedModelItemsID), modelCardID));
		return;
	}
	
	
		//Send the collected information
	auto result = std::make_unique<SendViaBrowserArgs>(*modelCard, *account, SendObject{std::move(element)});	//NB: Using a placeholder object for now
	getBridge()->sendEvent("sendByBrowser", std::move(result));
} //Send::run
