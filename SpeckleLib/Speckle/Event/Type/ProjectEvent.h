#ifndef SPECKLE_EVENT_PROJECT_EVENT
#define SPECKLE_EVENT_PROJECT_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

namespace speckle::event {
	
	/*!
	 Class representing a selection change event
	*/
	class ProjectEvent : public active::event::Event {
	public:
		
		// MARK: - Types
		
			///Common project event types
		enum class Type {
			unknown,
			newDocument,
			newAndReset,
			open,
			presave,
			save,
			close,
			quit,
			tempSave,
			send,
			receive,
		};
		
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
			m_eventID = convert(notifyEvent).value_or(Type::unknown);
			m_param = param;
		}
#endif
		
		/*!
		 Get the event type
		 @return The event type
		 */
		Type getType() const { return m_eventID; }
#ifdef ARCHICAD
		/*!
		 Get the event parameter
		 @return The event parameter
		 */
		int32_t getParam() const { return m_param; }
#endif

	private:
			///The event type
		Type m_eventID;
#ifdef ARCHICAD
			///An additional event parameter
		int32_t m_param;
		/*!
		 Convert an Archicad project event type
		 @return The equivalent Speckle event type
		 */
		static std::optional<ProjectEvent::Type> convert(API_NotifyEventID acEventType);
#endif
	};
	
}

#endif	//SPECKLE_EVENT_PROJECT_EVENT
