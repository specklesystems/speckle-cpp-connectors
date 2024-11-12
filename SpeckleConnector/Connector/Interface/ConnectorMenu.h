#ifndef CONNECTOR_CONNECTOR_MENU
#define CONNECTOR_CONNECTOR_MENU

#include "Speckle/Event/Subscriber/MenuSubscriber.h"

namespace connector {
	
	class ConnectorMenu : public speckle::event::MenuSubscriber {
	public:
		/*!
		 Default constructor
		 */
		ConnectorMenu();

		/*!
		 Get the event subscription list
		 @return The subscription list (an empty list will put the subscriber into a suspended state)
		 */
		Subscription subscription() const override;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Receive a subscribed event
		 @param event The incoming event
		 @return True if the event should be closed
		 */
		bool receive(const active::event::Event& event) override;
		
	protected:
		/*!
		 Handle the menu selection
		 @param event The menu event
		 @return True if the event should be closed
		 */
		bool handle(const speckle::event::MenuEvent& event) override;
	};
	
}

#endif //CONNECTOR_CONNECTOR_MENU
