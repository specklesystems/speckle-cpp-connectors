#ifndef SPECKLE_EVENT_SELECTION_SUBSCRIBER
#define SPECKLE_EVENT_SELECTION_SUBSCRIBER

#include "Active/Event/Subscriber.h"

namespace speckle::event {
	
	class SelectionEvent;
	
	/*!
	 Base class for subscribers responding to selection changes
	*/
	class SelectionSubscriber : public active::event::Subscriber {
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
		 Start the participant operation
		 @return True if the participant is able to continue
		 */
		virtual bool start() override;
		/*!
		 Handle the menu selection
		 @param event The menu event
		 @return True if the event should be closed
		 */
		virtual bool handle(const SelectionEvent& event) = 0;
		
	private:
			///True if a selection change subscriber has already started (only one is required - there are no variants)
		static bool m_isStarted;
	};
	
}

#endif	//SPECKLE_EVENT_SELECTION_SUBSCRIBER
