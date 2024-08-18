#ifndef SPECKLE_DATABASE_INDEX
#define SPECKLE_DATABASE_INDEX

#include "Speckle/Utility/Guid.h"

namespace speckle::database {
	
	/*!
	 Record index class
	 
	 A record index is the information required to uniquely identify (and locate) a specified record in some database/app context. For Archicad,
	 this is typically a guid, for Revit a string and for Vectorworks a handle. Note that this index is not necessarily persistent between
	 sessions.
	 */
	class Index : public speckle::utility::Guid {
	public:

		// MARK: - Types
		
		using base = speckle::utility::Guid;

		// MARK: - Constructors
		
		using base::base;
		
		Index(speckle::utility::Guid ID, speckle::utility::Guid dbaseID) : base{ID}, databaseID{dbaseID} {}
		
		// MARK: - Public variables
		
			///ID of the source database (not defined in every case, undefined = null guid)
		speckle::utility::Guid databaseID;
	};
	
}

#endif	//SPECKLE_DATABASE_INDEX
