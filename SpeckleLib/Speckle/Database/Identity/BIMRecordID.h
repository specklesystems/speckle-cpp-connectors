#ifndef SPECKLE_DATABASE_BIM_ID
#define SPECKLE_DATABASE_BIM_ID

#include "Speckle/Utility/Guid.h"

#include <unordered_set>

namespace speckle::database {
	
#ifdef ARCHICAD
		//Common BIM record identifier type (e.g. to BIM elements)
	using BIMRecordID = speckle::utility::Guid;
		//Common BIM table identifier type (e.g. primary model, views)
	using BIMTableID = speckle::utility::Guid;
		//Common BIM document identifier type (e.g. BIM drawing identifier, e.g. section, layout etc)
	using BIMDocID = speckle::utility::Guid;
		//Common BIM database identifier type (e.g. model database, library database, attribute database)
	using BIMDBaseID = speckle::utility::Guid;
		//Common BIM record identifier pair type (e.g. pairing a record with a parent)
	using BIMRecordIDPair = std::pair<speckle::utility::Guid, speckle::utility::Guid>;
#endif

		//A list of BIM record IDs
	using BIMRecordIDList = std::unordered_set<BIMRecordID>;
		//A list of BIM record ID pairs
	using BIMRecordIDPairList = std::unordered_set<BIMRecordIDPair>;
	
}

#endif	//SPECKLE_DATABASE_BIM_ID
