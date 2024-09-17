#ifndef SPECKLE_DATABASE_INDEX
#define SPECKLE_DATABASE_INDEX

#include "Active/Database/Identity/Link.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Utility/String.h"

#include <any>

namespace speckle::database {
	
	/*!
	 Record index class
	 
	 A record index is the information required to uniquely identify (and locate) a specified record in some database/app context. For Archicad,
	 this is typically a guid, for Revit a string and for Vectorworks a handle. Note that this index is not necessarily persistent between
	 sessions.
	 */
	class Index : public active::database::Index<RecordID> {
	public:

		// MARK: - Types
		
		using base = active::database::Index<RecordID>;

		// MARK: - Constructors
		
		using base::base;
	};
	
}

#endif	//SPECKLE_DATABASE_INDEX
