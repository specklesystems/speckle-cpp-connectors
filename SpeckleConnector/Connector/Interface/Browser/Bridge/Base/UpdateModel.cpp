#include "Connector/Interface/Browser/Bridge/Base/UpdateModel.h"

#include "Connector/Connector.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Interface/Browser/Bridge/Base/Arg/DocumentInfo.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::environment;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<PackageWrap, DocumentInfo>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
UpdateModel::UpdateModel() : BridgeMethod{"UpdateModel", [&](const ModelCardEventWrapper& card) {
		return run(card.get());
}} {}


/*--------------------------------------------------------------------
	Update a model card in document storage
 
	card: The card to update
  --------------------------------------------------------------------*/
void UpdateModel::run(const ModelCard& card) const {
	auto project = connector()->getActiveProject().lock();
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return;
	if (auto modelCardDBase = connectorProject->getModelCardDatabase(); modelCardDBase != nullptr) {
		auto newCard = clone(card);
		modelCardDBase->write(*newCard);
	}
} //UpdateModel::run
