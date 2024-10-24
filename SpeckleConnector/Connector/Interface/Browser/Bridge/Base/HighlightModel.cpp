#include "Connector/Interface/Browser/Bridge/Base/HighlightModel.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/Filter/SendFilter.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Record/Element/Element.h"

#include <array>

using namespace speckle::record::element;
using namespace connector::interfac::browser::bridge;
using namespace connector::record;
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
	Higlight the model card selection
 
	modelCardID: The ID of the model to send
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
	String className = typeid(*modelCard).name();
	if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
		auto modelCardSelection = senderCard->getFilter().getElementIDs();
#ifdef ARCHICAD	
		ACAPI_Selection_DeselectAll();
		GS::Array<API_Neig> selNeigs;
		for (const auto elemID : modelCardSelection) {
			API_Neig neig(elemID);
			selNeigs.Push(neig);
		}
		ACAPI_Selection_Select(selNeigs, true);
#endif
	}

} //HighlightModel::run
