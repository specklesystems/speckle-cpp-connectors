#ifndef SPECKLE_EVENT_SELECTION_EVENT
#define SPECKLE_EVENT_SELECTION_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"
#include "Speckle/Database/Identity/BIMLink.h"

namespace speckle::event {
	
	/*!
	 Class representing a selection change event
	*/
	class ElementChangedEvent : public active::event::Event {
	public:
		
		static const inline active::utility::NameID ID{active::utility::String{"element change"},
				active::utility::Guid{active::utility::String{"ac9366d5-90fd-497e-b7f7-a7b4c8d97c91"}}};

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param selected A link to a selected element (nullopt if the selection is empty)
		 */
		ElementChangedEvent(speckle::database::RecordIDList changed) : Event{ ID }, m_changedElementIDs{ changed } {}
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
		bool empty() const { return m_changedElementIDs.empty(); }
		/*!
		 Get a link to the last selected element
		 @return A link to the last selected element (nullopt if the event selection is empty)
		 */
		speckle::database::RecordIDList getChangedElementIDs() const { return m_changedElementIDs; }

	private:
		//speckle::database::BIMLink::Option m_changedLink;
		speckle::database::RecordIDList m_changedElementIDs;
		//RecordIDList
	};
	
}

#endif	//SPECKLE_EVENT_SELECTION_EVENT
