#ifndef SPECKLE_EVENT_SELECTION_EVENT
#define SPECKLE_EVENT_SELECTION_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"
#include "Speckle/Database/Content/Link.h"

namespace speckle::event {
	
	/*!
	 Class representing a selection change event
	*/
	class SelectionEvent : public active::event::Event {
	public:
		
		static const inline active::utility::NameID ID{active::utility::String{"selection change"},
				active::utility::Guid{active::utility::String{"b7fb1a43-dbab-4299-9eb5-2f93c68fbdaa"}}};

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param selected A link to a selected element (nullopt if the selection is empty)
		 */
		SelectionEvent(speckle::database::Link::Option selected) : m_selectedLink{selected} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		SelectionEvent(const SelectionEvent& source) = default;
		/*!
		 Destructor
		 */
		~SelectionEvent() = default;

		// MARK: - Functions (const)
		
		/*!
		 Determine if the event selection is empty
		 @return True if the event selection is empty
		 */
		bool empty() const { return !m_selectedLink; }
		/*!
		 Get a link to the last selected element
		 @return A link to the last selected element (nullopt if the event selection is empty)
		 */
		speckle::database::Link::Option getLastSelected() const { return m_selectedLink; }

	private:
		speckle::database::Link::Option m_selectedLink;
	};
	
}

#endif	//SPECKLE_EVENT_SELECTION_EVENT
