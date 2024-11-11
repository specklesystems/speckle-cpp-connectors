#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_BRIDGE
#define CONNECTOR_INTERFACE_BRIDGE_SEND_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Event/Subscriber/ElementSubscriber.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to support sending model data to a Speckle server
	*/
	class SendBridge : public speckle::interfac::browser::bridge::BrowserBridge, public speckle::event::ElementSubscriber {
	public:
		
		// MARK: - Types
		
		using base = speckle::interfac::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		using base::base;
		/*!
		 Default constructor
		 */
		SendBridge();

	protected:
		/*!
		 Handle an element change
		 @param event The selection event
		 @return True if the event should be closed
		 */
		bool handle(const speckle::event::ElementEvent& event) override;
	};
	
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_BRIDGE
