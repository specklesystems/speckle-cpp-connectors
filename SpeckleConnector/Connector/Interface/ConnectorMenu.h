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
