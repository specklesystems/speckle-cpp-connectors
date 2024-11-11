#ifndef SPECKLE_EVENT_SELECTION_SUBSCRIBER
#define SPECKLE_EVENT_SELECTION_SUBSCRIBER

#include "Active/Event/Subscriber.h"

namespace speckle::event {
	
	class SelectionEvent;
	
	/*!
	 Base class for subscribers responding to selection changes
	*/
	class SelectionSubscriber : public virtual active::event::Subscriber {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SelectionSubscriber() = default;
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		SelectionSubscriber(const SelectionSubscriber& source) = default;
		/*!
		 Destructor
		 */
		~SelectionSubscriber() = default;

		// MARK: - Functions (const)
		
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
		/*!
		 Start the participant operation
		 @return True if the participant is able to continue
		 */
		virtual bool start() override;
		/*!
		 Stop participation (release resources etc)
		 */
		void stop() override;
		
	protected:

		/*!
		 Handle a selection change
		 @param event The selection event
		 @return True if the event should be closed
		 */
		virtual bool handle(const SelectionEvent& event) = 0;
	};
	
}

#endif	//SPECKLE_EVENT_SELECTION_SUBSCRIBER
