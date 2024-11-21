#ifndef CONNECTOR_EVENT_ID
#define CONNECTOR_EVENT_ID

#include "Active/Utility/NameID.h"

// MARK: - Event identities

	//Identifier for event signalling the connector palette visibility should be toggled
inline const active::utility::NameID toggleConnectorPaletteID{"toggleConnectorPalette"};
	//Identifier for event signalling the connector palette has changed (state carried with event)
inline const active::utility::NameID reflectPaletteVisibilityID{"setConnectorMenuCheck"};
	//Identifier for event signalling the that an attached list of elements should be highlighted
inline const active::utility::NameID setElementHighlight{"setElementHighlight"};

// MARK: - Event setting identities

	//Identifier for setting carrying a menu state
inline const active::utility::NameID paletteVisibilityStateID{"menuCheckState"};
	//Identifier for setting carrying a list of element links
inline const active::utility::NameID recordLinks{"recordLinks"};

#endif	//CONNECTOR_EVENT_ID
