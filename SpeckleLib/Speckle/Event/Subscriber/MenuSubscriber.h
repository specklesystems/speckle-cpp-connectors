#ifndef SPECKLE_EVENT_MENU_SUBSCRIBER
#define SPECKLE_EVENT_MENU_SUBSCRIBER

#include "Active/Event/Subscriber.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

namespace speckle::event {
	
	class MenuEvent;

#ifdef ARCHICAD
		///Menu locations in the host UI
	using MenuLocation = APIMenuCodeID;	//Locations are defined in Archicad
	constexpr APIMenuCodeID userDefinedMenu = MenuCode_UserDef;	//Alias the user-defined location for platform independence
#else
		///Menu locations in the host UI - can be expanded in future as required
	enum MenuLocation {
		userDefinedMenu = 0;	///<Let the user decide where the item appears
	};
#endif

	/*!
	 Base class for subscribers handling a UI menu (from the app menu bar)
	*/
	class MenuSubscriber : public active::event::Subscriber {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		MenuSubscriber() = default;
		/*!
		 Constructor
		 @param resourceID ID for resources linked to the menu
		 @param helpID ID for resources linked to help
		 @param location The menu location in the UI (defaults to user-defined)
		 @param flags Flags linked to menu behaviour
		 */
		MenuSubscriber(short resourceID, short helpID = 0, MenuLocation location = userDefinedMenu, uint32_t flags = 0) :
				m_resourceID{resourceID}, m_helpID{helpID}, m_flags{flags}, m_location{location} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		MenuSubscriber(const MenuSubscriber& source) = default;
		/*!
		 Destructor
		 */
		~MenuSubscriber() = default;

		// MARK: - Functions (const)
		
		/*!
		 Get the event subscription list
		 @return The subscription list (an empty list will put the subscriber into a suspended state)
		*/
		virtual Subscription subscription() const override;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Receive a subscribed event
		 @param event The incoming event
		 @return True if the event should be closed
		 */
		virtual bool receive(const active::event::Event& event) override;
		
	protected:
		/*!
		 Attach participant components to the app (as required)
		 @return True if the participant is able to function
		 */
		virtual bool attach() override;
		/*!
		 Start the participant operation
		 @return True if the participant is able to continue
		 */
		virtual bool start() override;
		/*!
		 Handle the menu selection
		 @param event The menu event
		 @return True if the event should be closed
		 */
		virtual bool handle(const MenuEvent& event) = 0;
		
	private:
			///The menu resource ID
		short m_resourceID = 0;
			///The help resource ID
		short m_helpID = 0;
			///Flags linked to menu behaviour
		uint32_t m_flags = 0;
			///The menu location in the UI
		MenuLocation m_location = userDefinedMenu;
	};
	
}

#endif	//SPECKLE_EVENT_MENU_SUBSCRIBER
