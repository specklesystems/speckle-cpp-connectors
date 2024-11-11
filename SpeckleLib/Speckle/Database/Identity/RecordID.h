#ifndef SPECKLE_DATABASE_ID
#define SPECKLE_DATABASE_ID

#include "Speckle/Utility/String.h"
#include "Speckle/Utility/Guid.h"

namespace speckle::database {
	
		//Common Speckle record identifier type
	using RecordID = speckle::utility::String;
		//Common Speckle table identifier type
	using TableID = speckle::utility::String;
		//Common Speckle database identifier type
	using DBaseID = speckle::utility::String;

		//A list of record IDs
	using RecordIDList = std::vector<RecordID>;

	//BIM element record identifier
	using ElementID = speckle::utility::Guid;

	//A list of element IDs
	using ElementIDList = std::vector<ElementID>;
	
}

#endif	//SPECKLE_DATABASE_ID
