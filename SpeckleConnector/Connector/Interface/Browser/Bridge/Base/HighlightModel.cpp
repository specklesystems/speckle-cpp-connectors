#include "Connector/Interface/Browser/Bridge/Base/HighlightModel.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/Filter/SendFilter.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"

using namespace speckle::record::element;
using namespace connector::interfac::browser::bridge;
using namespace connector::record;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
HighlightModel::HighlightModel() : BridgeMethod{"HighlightModel", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Highlight elements linked to the model card selection
 
	modelCardID: The ID of the target model card
  --------------------------------------------------------------------*/
void HighlightModel::run(const String& modelCardID) const {
	// Find the specified model card
	auto modelCardDatabase = connector()->getModelCardDatabase();
	auto modelCard = modelCardDatabase->getCard(modelCardID);
	if (!modelCard) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, modelCardNotFoundID), modelCardID));
		return;
	}
	
	if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
		auto modelCardSelection = senderCard->getFilter().getElementIDs();

		auto project = connector()->getActiveProject().lock();
		if (!project) {
			// TODO: is this OK? should this throw?
			return;
		}

		auto elementDatabase = project->getElementDatabase();
		elementDatabase->clearSelection();
		elementDatabase->setSelection(modelCardSelection);
	}

} //HighlightModel::run
