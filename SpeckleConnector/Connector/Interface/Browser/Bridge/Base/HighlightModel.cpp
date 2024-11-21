#include "Active/Setting/ValueSetting.h"
#include "Active/Setting/Values/GuidValue.h"
#include "Active/Event/Event.h"
#include "Connector/Interface/Browser/Bridge/Base/HighlightModel.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Event/ConnectorEventID.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/Filter/SendFilter.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Host.h"
#include "Speckle/Environment/Project.h"

using namespace active::event;
using namespace active::setting;
using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace connector::record;
using namespace speckle::environment;
using namespace speckle::record::element;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
HighlightModel::HighlightModel() : BridgeMethod{"HighlightModel", [&](const HighlightModelArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Highlight elements linked to the model card selection
 
	modelCardID: The ID of the target model card
  --------------------------------------------------------------------*/
void HighlightModel::run(const String& modelCardID) const {
	auto project = connector()->getActiveProject().lock();
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
	if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
		ValueSetting elementIDs{recordLinks};
		for (const auto& elementID : senderCard->getFilter().getElementIDs())
			elementIDs.emplace_back(GuidValue{elementID});
		connector()->publish(Event{setElementHighlight, { elementIDs }});
	}
} //HighlightModel::run
