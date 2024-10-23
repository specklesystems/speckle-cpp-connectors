#include "Connector/Interface/Browser/Bridge/Base/HighlightModel.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendViaBrowserArgs.h"
#include "Connector/Record/Collection/ProjectCollection.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/ReceiverModelCard.h"
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
HighlightModel::HighlightModel() : BridgeMethod{"HighlightModel", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the model to send
  --------------------------------------------------------------------*/
void HighlightModel::run(const String& modelCardID) const {
		//Find the specified model card
	auto modelCardDatabase = connector()->getModelCardDatabase();
	auto modelCard = modelCardDatabase->getCard(modelCardID);
	//connector::record::ModelCard::Unique modelCard = modelCardDatabase->getCard(modelCardID);
	if (!modelCard) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, modelCardNotFoundID), modelCardID));
		return;
	}
	String className = typeid(*modelCard).name();
	if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
		auto x = senderCard->getFilter().getElementIDs();
	}

	if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
		int i = 0;
	}
	else if (auto receiverCard = dynamic_cast<ReceiverModelCard*>(modelCard.get())) {
		int i = 0;
	}
	else {
		int i = 0;
	}

} //HighlightModel::run
