#ifndef SPECKLE_EVENT_PROJECT_EVENT
#define SPECKLE_EVENT_PROJECT_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"

namespace speckle::event {
	
	/*!
	 Class representing a selection change event
	*/
	class ProjectEvent : public active::event::Event {
	public:
		
		static const inline active::utility::NameID ID{active::utility::String{"project event"},
				active::utility::Guid{active::utility::String{"0ffb9ec5-2164-4fc2-aa57-17b5a1f15355"}}};

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ProjectEvent() : Event{ID} {}
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param notifyEvent The project event type
		 @param param An additional parameter relevant to some project events
		 */
		ProjectEvent(API_NotifyEventID notifyEvent, int32_t param) : Event{ID} {
			m_eventID = notifyEvent;
			m_param = param;
		}
		
		/*!
		 Get the event type
		 @return The event type
		 */
		API_NotifyEventID getType() const { return m_eventID; }
		/*!
		 Get the event parameter
		 @return The event parameter
		 */
		int32_t getParam() const { return m_param; }
#endif

#ifdef ARCHICAD
			//Incoming document objects to merge
		API_NotifyEventID m_eventID;
		int32_t m_param;
#endif
	};
	
}

#endif	//SPECKLE_EVENT_PROJECT_EVENT
