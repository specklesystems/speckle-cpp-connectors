#ifndef SPECKLE_DATABASE_INDEX
#define SPECKLE_DATABASE_INDEX

#include "Speckle/Utility/Guid.h"

namespace speckle::database {
	
		///The record index type for the local context - Guid is suitable for Archicad, but update as other contexts are introduced
	using RecordIndex = speckle::utility::Guid;
		///The database index type for the local context - Guid is suitable for Archicad, but update as other contexts are introduced
	using DBaseIndex = speckle::utility::Guid;

	/*!
	 Record index class
	 
	 A record index is the information required to uniquely identify (and locate) a specified record in some database/app context. For Archicad,
	 this is typically a guid, for Revit a string and for Vectorworks a handle. Note that this index is not necessarily persistent between
	 sessions.
	 */
	class Index : public RecordIndex {
	public:

		// MARK: - Types
		
		using base = RecordIndex;

		// MARK: - Constructors
		
		using base::base;
		
		// MARK: - Public variables
		
			///ID of the source database (not defined in every case, undefined = null guid)
		DBaseIndex databaseID;
	};
	
}

#endif	//SPECKLE_DATABASE_INDEX
