#ifndef CONNECTOR_DATABASE_ID
#define CONNECTOR_DATABASE_ID

#include "Speckle/Utility/Guid.h"

namespace connector::database {
	
		//BIM element record identifier
	using ElementID = speckle::utility::Guid;

		//A list of element IDs
	using ElementIDList = std::vector<ElementID>;
	
}

#endif	//CONNECTOR_DATABASE_ID
