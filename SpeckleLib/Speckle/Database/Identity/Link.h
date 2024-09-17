#ifndef SPECKLE_DATABASE_LINK
#define SPECKLE_DATABASE_LINK

#include "Active/Setting/SettingList.h"
#include "Speckle/Database/Identity/Index.h"

namespace speckle::database {

	/*!
	 Record link class, binding an index to additional information
	 
	 The essential part of a link is the index pointing to a target record. In some cases, e.g. where the user has selected something in a rendered
	 scene, the index is to a proxy object that represents another record in a specific context, e.g. a 2D representation of a 3D element rendered
	 in a cut plance section view. A tool working with the link might be interested in either the proxy or the original - passing a link allows
	 this distinction to be easily made.
	 
	 A link may optionally carry any number of settings. In the context of a user selection (for example) there might be settings describing where
	 the user made the selection (e.g. the hole in a floor slab), allowing a tool working on that selection to be more precise.
	 */
	class Link : public active::database::Link<RecordID> {
	public:
		
		// MARK: - Types
		
		using base = active::database::Link<RecordID>;
		
		// MARK: - Constructors
		
		using base::base;
		
		Link() = default;
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param selected Information about a selected Archicad element
		 */
		Link(const API_Neig& selected);
#endif
	};
	
}

#endif	//SPECKLE_DATABASE_LINK
