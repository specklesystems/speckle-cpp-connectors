#ifndef SPECKLE_EVENT_PROJECT_SUBSCRIBER
#define SPECKLE_EVENT_PROJECT_SUBSCRIBER

#include "Active/Event/Subscriber.h"

namespace speckle::event {
	
	class ProjectEvent;
	
	/*!
	 Base class for subscribers responding to project events (open, close, quit etc)
	*/
	class ProjectSubscriber : public virtual active::event::Subscriber {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param priority The subscriber priority (determines the order in which subscribers receive events)
		*/
		ProjectSubscriber(int32_t priority = 0) : active::event::Subscriber{priority} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ProjectSubscriber(const ProjectSubscriber& source) = default;
		/*!
		 Destructor
		 */
		virtual ~ProjectSubscriber() {}

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
		bool start() override;
		
	protected:
		/*!
		 Handle a project event
		 @param event The project event
		 @return True if the event should be closed
		 */
		virtual bool handle(const ProjectEvent& event) = 0;
		
	private:
			///True if a doc merge subscriber has already started (only one is required - there are no variants)
		static bool m_isStarted;
	};
	
}

#endif	//SPECKLE_EVENT_PROJECT_SUBSCRIBER
