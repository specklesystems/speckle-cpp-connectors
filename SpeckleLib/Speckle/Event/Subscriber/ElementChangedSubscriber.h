#ifndef SPECKLE_EVENT_ELEMENT_CHANGED_SUBSCRIBER
#define SPECKLE_EVENT_ELEMENT_CHANGED_SUBSCRIBER

#include "Active/Event/Subscriber.h"
#include "Speckle/Database/Identity/RecordID.h"

namespace speckle::event {
	
	class ElementChangedEvent;
	
	/*!
	 Base class for subscribers responding to element changes
	*/
	class ElementChangedSubscriber : public virtual active::event::Subscriber {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ElementChangedSubscriber() = default;
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ElementChangedSubscriber(const ElementChangedSubscriber& source) = default;
		/*!
		 Destructor
		 */
		~ElementChangedSubscriber() = default;

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
		 Handle an element change
		 @param event The element change event
		 @return True if the event should be closed
		 */
		virtual bool handle(const ElementChangedEvent& event) = 0;

		speckle::database::ElementIDList m_changedElements;
	};
	
}

#endif	//SPECKLE_EVENT_ELEMENT_CHANGED_SUBSCRIBER
