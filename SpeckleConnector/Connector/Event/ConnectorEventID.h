#ifndef CONNECTOR_EVENT_ID
#define CONNECTOR_EVENT_ID

#include "Active/Utility/NameID.h"

// MARK: - Event identities

	//Identifier for event signalling the connector palette visibility should be toggled
inline const active::utility::NameID toggleConnectorPaletteID{"toggleConnectorPalette"};
	//Identifier for event signalling the connector palette menu checked state should be set (state carried with event)
inline const active::utility::NameID setConnectorMenuCheckID{"setConnectorMenuCheck"};

// MARK: - Event setting identities

	//Identifier for event signalling the connector palette menu checked state should be set (state carried with event)
inline const active::utility::NameID menuCheckStateID{"menuCheckState"};

#endif	//CONNECTOR_EVENT_ID
