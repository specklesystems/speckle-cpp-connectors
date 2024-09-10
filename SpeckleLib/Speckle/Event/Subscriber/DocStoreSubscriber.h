#ifndef SPECKLE_EVENT_DOC_STORE_SUBSCRIBER
#define SPECKLE_EVENT_DOC_STORE_SUBSCRIBER

#include "Active/Event/Subscriber.h"

namespace speckle::event {
	
	class DocStoreMergeEvent;
	
	/*!
	 Base class for subscribers responding to document merge operations (notably those managing custom data in the document)
	*/
	class DocStoreSubscriber : public active::event::Subscriber {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		DocStoreSubscriber() = default;
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		DocStoreSubscriber(const DocStoreSubscriber& source) = default;
		/*!
		 Destructor
		 */
		virtual ~DocStoreSubscriber() {}

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
		
	protected:
		/*!
		 Attach participant components to the app (as required)
		 @return True if the participant is able to function
		*/
		bool attach() override;
		/*!
		 Start the participant operation
		 @return True if the participant is able to continue
		 */
		bool start() override;
		/*!
		 Handle a document merge operation
		 @param event The merge event
		 @return True if the event should be closed
		 */
		virtual bool handle(const DocStoreMergeEvent& event) = 0;
		
	private:
			///True if a doc merge subscriber has already started (only one is required - there are no variants)
		static bool m_isStarted;
	};
	
}

#endif	//SPECKLE_EVENT_DOC_STORE_SUBSCRIBER
