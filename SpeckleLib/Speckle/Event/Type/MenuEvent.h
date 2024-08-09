#ifndef SPECKLE_EVENT_MENU_EVENT
#define SPECKLE_EVENT_MENU_EVENT

#include "Active/Event/Event.h"

#include "Active/Utility/Guid.h"
#include "Active/Utility/String.h"

namespace speckle::event {
	
	/*!
	 Class representing a menu event
	*/
	class MenuEvent : public active::event::Event {
	public:
		
		static const inline active::utility::NameID ID{active::utility::String{"menu bar item click"},
				active::utility::Guid{active::utility::String{"5a6a2ceb-f4c7-4853-be97-fda802c78d65"}}};

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param menu The ID of the target menu for the event
		 @param item The ID of the clicked menu item
		 */
		MenuEvent(int32_t menu, int32_t item) : Event{ID}, menuID{menu}, itemID{item} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		MenuEvent(const MenuEvent& source) = default;
		/*!
		 Destructor
		 */
		~MenuEvent() = default;

		// MARK: - Public variables
		
			///The menu identifier
		const int32_t menuID = 0;
			///The clicked menu item identifier
		const int32_t itemID = 0;
	};
	
}

#endif	//SPECKLE_EVENT_MENU_EVENT
