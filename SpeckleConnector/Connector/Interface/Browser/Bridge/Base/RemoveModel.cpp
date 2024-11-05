#include "Connector/Interface/Browser/Bridge/Base/RemoveModel.h"

#include "Connector/Connector.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Database/ModelCardDatabase.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::environment;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
RemoveModel::RemoveModel() : BridgeMethod{"RemoveModel", [&](const ModelCardEventWrapper& card) {
		return run(card.get());
}} {}


/*--------------------------------------------------------------------
	Add a model card to document storage
 
	card: The card to add
  --------------------------------------------------------------------*/
void RemoveModel::run(const ModelCard& card) const {
	auto project = connector()->getActiveProject().lock();
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return;
	if (auto modelCardDBase = connectorProject->getModelCardDatabase(); modelCardDBase != nullptr)
		modelCardDBase->erase(card.getID());
} //RemoveModel::run
