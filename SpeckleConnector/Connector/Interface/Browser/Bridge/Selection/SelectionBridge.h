#ifndef CONNECTOR_INTERFACE_BRIDGE_SELECTION_BRIDGE
#define CONNECTOR_INTERFACE_BRIDGE_SELECTION_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Event/Subscriber/SelectionSubscriber.h"
#include "Speckle/Event/Type/SelectionEvent.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to manage element selection information passed between the JS UI and the BIM application
	*/
	class SelectionBridge : public speckle::interfac::browser::bridge::BrowserBridge, public speckle::event::SelectionSubscriber {
	public:
		
		// MARK: - Types
		
		using base = speckle::interfac::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		using base::base;
		/*!
		 Default constructor
		 */
		SelectionBridge();

	protected:
		/*!
		 Handle a selection change
		 @param event The selection event
		 @return True if the event should be closed
		 */
		bool handle(const speckle::event::SelectionEvent& event) override;
	};
	
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SELECTION_BRIDGE
