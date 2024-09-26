#ifndef SPECKLE_DATABASE_BIM_INDEX
#define SPECKLE_DATABASE_BIM_INDEX

#include "Active/Database/Identity/Index.h"
#include "Speckle/Database/Identity/RecordID.h"

namespace speckle::database {
	
	/*!
	 Record index class
	 
	 A record index is the information required to uniquely identify (and locate) a specified record in some database/app context. For Archicad,
	 this is typically a guid, for Revit a string and for Vectorworks a handle. Note that this index is not necessarily persistent between
	 sessions.
	 */
	class BIMIndex : public active::database::Index<BIMRecordID> {
	public:

		// MARK: - Types
		
		using base = active::database::Index<BIMRecordID>;

		// MARK: - Constructors
		
		using base::base;
		
		// MARK: - Public variables
		
			//The table identifier
		BIMTableID table;
			//The document identifier
		BIMDocID document;
	};
	
}

#endif	//SPECKLE_DATABASE_BIM_INDEX
