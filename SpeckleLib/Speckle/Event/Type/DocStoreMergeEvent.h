#ifndef SPECKLE_EVENT_DOC_STORE_MERGE_EVENT
#define SPECKLE_EVENT_DOC_STORE_MERGE_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"
#include "Speckle/Database/Identity/Link.h"

namespace speckle::event {
	
	/*!
	 Class representing a selection change event
	*/
	class DocStoreMergeEvent : public active::event::Event {
	public:
		
		static const inline active::utility::NameID ID{active::utility::String{"document store merge"},
				active::utility::Guid{active::utility::String{"c92e2c51-d47c-44e3-a54f-5068dccaa35a"}}};

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		DocStoreMergeEvent() : Event{ID} {}
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param incoming Incoming document objects to merge
		 */
		DocStoreMergeEvent(const GS::Array<API_AddonObject>& incoming) : Event{ID} { objects = &incoming; }
#endif

#ifdef ARCHICAD
			//Incoming document objects to merge
		const GS::Array<API_AddonObject>* objects = nullptr;
#endif
	};
	
}

#endif	//SPECKLE_EVENT_DOC_STORE_MERGE_EVENT
