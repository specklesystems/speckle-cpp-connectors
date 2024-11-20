#ifndef SPECKLE_EVENT_ELEMENT_EVENT
#define SPECKLE_EVENT_ELEMENT_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::event {
	
	/*!
	 Class representing an event signaling a database transaction operating on a BIM element, e.g. adding, editing, deleting etc
	*/
	class ElementEvent : public active::event::Event {
	public:

		enum Type {
			newElem,	///<A new element has been created
			begin,	///<An element database transaction has started - subsequent events will detail specific actions
			end,	///<The current element database transaction has ended - any handling based on the transaction events should be fone now
			changeElem,	///<An element has been changed
			editElem,	///<An element has been edited
			deleteElem	///<An element has been deleted
		};
		
		static const inline active::utility::NameID ID{active::utility::String{"element transaction"},
				active::utility::Guid{active::utility::String{"ac9366d5-90fd-497e-b7f7-a7b4c8d97c91"}}};

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param eventType An event type identifier
		 @param targetID The ID of the element targeted by the database transaction (nullopt = undefined, e.g. for a begin/end event)
		 */
		ElementEvent(Type eventType, database::BIMRecordID::Option targetID = std::nullopt) : Event{ID}, m_elementID{targetID}, m_eventType{eventType} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ElementEvent(const ElementEvent& source) = default;
		/*!
		 Destructor
		 */
		~ElementEvent() = default;

		// MARK: - Functions (const)
		
		/*!
		 Get the ID of the database transaction target element
		 @return The target element ID (nullopt = no target, e.g. a begin/end event)
		 */
		database::BIMRecordID::Option getElementID() const { return m_elementID; }
		Type getEventType() const { return m_eventType; }

	private:
			///The ID of the target element (nullopt = undefined)
		database::BIMRecordID::Option m_elementID;
			///The specific database operation performed
		Type m_eventType;
	};
	
}

#endif	//SPECKLE_EVENT_ELEMENT_EVENT
