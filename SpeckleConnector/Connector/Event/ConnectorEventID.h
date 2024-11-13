#ifndef CONNECTOR_EVENT_ID
#define CONNECTOR_EVENT_ID

#include "Active/Utility/NameID.h"

// MARK: - Event identities

	//Identifier for event signalling the connector palette visibility should be toggled
inline const active::utility::NameID toggleConnectorPaletteID{"toggleConnectorPalette"};
	//Identifier for event signalling the connector palette has changed (state carried with event)
inline const active::utility::NameID reflectPaletteVisibilityID{"setConnectorMenuCheck"};

// MARK: - Event setting identities

	//Identifier for event signalling the connector palette visibility state
inline const active::utility::NameID paletteVisibilityStateID{"menuCheckState"};

#endif	//CONNECTOR_EVENT_ID
