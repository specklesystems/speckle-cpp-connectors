#include "Connector/Interface/Browser/Bridge/Base/HighlightObjects.h"

#include "Active/Setting/ValueSetting.h"
#include "Active/Event/Event.h"
#include "Connector/Connector.h"
#include "Connector/Event/ConnectorEventID.h"

using namespace active::event;
using namespace active::setting;
using namespace connector;
using namespace connector::interfac::browser::bridge;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
HighlightObjects::HighlightObjects() : BridgeMethod{"HighlightObjects", [&](const HighlightObjectArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Highlight specified objects

	objectIDs: List of object IDs to be highlighted
  --------------------------------------------------------------------*/
void HighlightObjects::run(const StringList& objectIDs) const {
	connector()->publish(Event{setElementHighlight, { ValueSetting{objectIDs, recordLinks} }});
} //HighlightObjects::run
