#ifndef CONNECTOR_CONNECTOR_PALETTE
#define CONNECTOR_CONNECTOR_PALETTE

#include "Active/Event/Subscriber.h"

namespace connector {
	
	class ConnectorPalette : public active::event::Subscriber {
	public:
		/*!
		 Default constructor
		 */
		ConnectorPalette();
		
		// MARK: - Functions (const)
		
		/*!
			Get the event subscription list
			@return The subscription list (an empty list will put the subscriber into a suspended state)
		*/
		Subscription subscription() const override;
		
		// MARK: - Functions (mutating)
		/*!
		 Start the participant operation
		 @return True if the participant is able to continue
		 */
		bool start() override;
		/*!
			Receive a subscribed event
			@param event The incoming event
			@return True if the event should be closed
		*/
		bool receive(const active::event::Event& event) override;
	};
	
}

#endif //CONNECTOR_CONNECTOR_PALETTE
