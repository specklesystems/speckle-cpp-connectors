#ifndef CONNECTOR_INTERFACE_BRIDGE_BASE_BRIDGE
#define CONNECTOR_INTERFACE_BRIDGE_BASE_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Event/Subscriber/ProjectSubscriber.h"
#include "Speckle/Event/Type/ProjectEvent.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to provide configuration settings
	*/
	class BaseBridge : public speckle::interfac::browser::bridge::BrowserBridge, public speckle::event::ProjectSubscriber {
	public:
		
		// MARK: - Types
		
		using base = speckle::interfac::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		using base::base;
		/*!
		 Default constructor
		 */
		BaseBridge();

	protected:
		/*!
		 Handle the project events
		 @param event The project event
		 @return True if the event should be closed
		 */
		bool handle(const speckle::event::ProjectEvent& event) override;
	};
	
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_BASE_BRIDGE
