#include "Connector/Interface/Browser/Bridge/Base/HighlightObjects.h"

#include "Connector/Connector.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"

using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace speckle::database;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
HighlightObjects::HighlightObjects() : BridgeMethod{"HighlightObjects", [&](const HighlightObjectArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Highlight specified objects
	@param objectIDs List of object IDs to be highlighted
  --------------------------------------------------------------------*/
void HighlightObjects::run(const StringList& objectIDs) const {
	BIMLinkList objectSelection;
	for (const auto& text : objectIDs)
		if (Guid guid{text}; !guid.empty())
			objectSelection.emplace_back(guid);
	if (objectSelection.empty())
		return;
	auto project = connector()->getActiveProject().lock();
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return;
	auto elementDatabase = project->getElementDatabase();
	elementDatabase->clearSelection();
	elementDatabase->setSelection(objectSelection);
} //HighlightObjects::run
