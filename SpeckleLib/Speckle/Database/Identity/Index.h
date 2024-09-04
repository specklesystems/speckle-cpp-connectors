#ifndef SPECKLE_DATABASE_INDEX
#define SPECKLE_DATABASE_INDEX

#include "Active/Database/Identity/Link.h"
#include "Speckle/Utility/String.h"

namespace speckle::database {
	
	/*!
	 Record index class
	 
	 A record index is the information required to uniquely identify (and locate) a specified record in some database/app context. For Archicad,
	 this is typically a guid, for Revit a string and for Vectorworks a handle. Note that this index is not necessarily persistent between
	 sessions.
	 */
	class Index : public active::database::Index<speckle::utility::String, speckle::utility::String, speckle::utility::String> {
	public:

		// MARK: - Types
		
		using base = active::database::Index<speckle::utility::String, speckle::utility::String, speckle::utility::String>;

		// MARK: - Constructors
		
		using base::base;
	};
	
}

#endif	//SPECKLE_DATABASE_INDEX
