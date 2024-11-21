#ifndef CONNECTOR_ELEMENT_HIGHLIGHTER
#define CONNECTOR_ELEMENT_HIGHLIGHTER

#include "Active/Event/Subscriber.h"

namespace connector {
	
	class ElementHighlighter : public active::event::Subscriber {
	public:
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
	};
	
}

#endif //CONNECTOR_ELEMENT_HIGHLIGHTER
