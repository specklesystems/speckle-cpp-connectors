#ifndef SPECKLE_DATABASE_LINK
#define SPECKLE_DATABASE_LINK

#include "Active/Setting/SettingList.h"
#include "Speckle/Database/Content/Index.h"

namespace speckle::database {
	
		///The record index type for the local context - Guid is suitable for Archicad, but update as other contexts are introduced
	using RecordIndex = speckle::utility::Guid;
		///The database index type for the local context - Guid is suitable for Archicad, but update as other contexts are introduced
	using DBaseIndex = speckle::utility::Guid;

	/*!
	 Record link class, binding an index to additional information
	 
	 The essential part of a link is the index pointing to a target record. In some cases, e.g. where the user has selected something in a rendered
	 scene, the index is to a proxy object that represents another record in a specific context, e.g. a 2D representation of a 3D element rendered
	 in a cut plance section view. A tool working with the link might be interested in either the proxy or the original - passing a link allows
	 this distinction to be easily made.
	 
	 A link may optionally carry any number of settings. In the context of a user selection (for example) there might be settings describing where
	 the user made the selection (e.g. the hole in a floor slab), allowing a tool working on that selection to be more precise.
	 */
	class Link : public active::setting::SettingList {
	public:

		// MARK: - Types
		
		using base = active::setting::SettingList;
			///Unique pointer
		using Unique = std::unique_ptr<Link>;
			///Shared pointer
		using Shared = std::shared_ptr<Link>;
			///Optional
		using Option = std::optional<Link>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Constructor
		 @param index An element index
		 @param origIndex The index of the original element (when the preceding index is to a proxy element)
		 */
		Link(const Index& index, const Index& origIndex = Index{}) : recordID{index}, originalID{origIndex} {}
		
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param selected Information about a selected Archicad element
		 */
		Link(const API_Neig& selected);
#endif
		
		// MARK: - Public variables
		
			///ID of the linked record (defaults to null guid = undefined)
		Index recordID;
			///ID of the original record (in the case where the record is proxy for another - null guid = undefined)
		Index originalID;
	};
	
}

#endif	//SPECKLE_DATABASE_LINK
