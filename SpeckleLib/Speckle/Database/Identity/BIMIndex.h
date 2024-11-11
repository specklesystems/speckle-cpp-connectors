#ifndef SPECKLE_DATABASE_BIM_INDEX
#define SPECKLE_DATABASE_BIM_INDEX

#include "Active/Database/Identity/Index.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::database {
	
	/*!
	 Record index class
	 
	 A record index is the information required to uniquely identify (and locate) a specified record in some database/app context. For Archicad,
	 this is typically a guid, for Revit a string and for Vectorworks a handle. Note that this index is not necessarily persistent between
	 sessions.
	 */
	class BIMIndex : public active::database::Index<BIMRecordID, BIMRecordID> {
	public:

		// MARK: - Types
		
		using base = active::database::Index<BIMRecordID, BIMRecordID>;

		// MARK: - Constructors
		
		using base::base;
	};
	
}


	///Hashing for BIMIndex class, e.g. to use as a key in unordered_map
template<>
struct std::hash<speckle::database::BIMIndex> {
	std::size_t operator() (const speckle::database::BIMIndex& index) const {
		std::size_t h1 = std::hash<active::utility::Guid>{}(index);
		std::size_t h2 = std::hash<active::utility::Guid>{}(index.tableID);
		return h1 ^ (h2 << 1);
	}
};


#endif	//SPECKLE_DATABASE_BIM_INDEX
