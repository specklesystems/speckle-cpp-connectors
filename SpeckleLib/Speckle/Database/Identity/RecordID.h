#ifndef SPECKLE_DATABASE_ID
#define SPECKLE_DATABASE_ID

#include "Speckle/Utility/String.h"

namespace speckle::database {
	
		//Common Speckle record identifier type
	using RecordID = speckle::utility::String;
		//Common Speckle table identifier type
	using TableID = speckle::utility::String;
		//Common Speckle database identifier type
	using DBaseID = speckle::utility::String;

		//A list of record IDs
	using RecordIDList = std::vector<RecordID>;
	
}

#endif	//SPECKLE_DATABASE_ID
