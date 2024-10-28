#ifndef SPECKLE_EVENT_SELECTION_EVENT
#define SPECKLE_EVENT_SELECTION_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"

namespace speckle::event {
	
	/*!
	 Class representing a selection change event
	*/
	class ElementChangedEvent : public active::event::Event {
	public:

		enum EventType { New, Begin, End, Change, Edit };
		
		static const inline active::utility::NameID ID{active::utility::String{"element change"},
				active::utility::Guid{active::utility::String{"ac9366d5-90fd-497e-b7f7-a7b4c8d97c91"}}};

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param selected A link to a selected element (nullopt if the selection is empty)
		 */
		ElementChangedEvent(speckle::database::ElementID changed, EventType eventType) : Event{ ID }, m_changedElement{ changed }, m_eventType{ eventType } {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ElementChangedEvent(const ElementChangedEvent& source) = default;
		/*!
		 Destructor
		 */
		~ElementChangedEvent() = default;

		// MARK: - Functions (const)
		
		/*!
		 Determine if the event selection is empty
		 @return True if the event selection is empty
		 */
		bool empty() const { return m_changedElement.empty(); }
		/*!
		 Get a link to the last selected element
		 @return A link to the last selected element (nullopt if the event selection is empty)
		 */
		speckle::database::ElementID getChangedElement() const { return m_changedElement; }
		EventType getEventType() const { return m_eventType; }

	private:
		speckle::database::ElementID m_changedElement;
		EventType m_eventType;
	};
	
}

#endif	//SPECKLE_EVENT_SELECTION_EVENT
