#include "Connector/Interface/Browser/Bridge/Base/GetDocumentState.h"

#include "Connector/Connector.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Database/ModelCardDatabase.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetDocumentState::GetDocumentState() : BridgeMethod{"GetDocumentState", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the document model cards
 
	return: The document model cards
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetDocumentState::run() const {
	auto project = connector()->getActiveProject().lock();
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return nullptr;
	if (auto modelCardDBase = connectorProject->getModelCardDatabase(); modelCardDBase != nullptr) {
		return modelCardDBase->wrapper();
	}
	return nullptr;
} //GetDocumentState::run
