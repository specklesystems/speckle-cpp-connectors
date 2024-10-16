#ifndef CONNECTOR_INTERFACE_BRIDGE_SELECTION_BRIDGE
#define CONNECTOR_INTERFACE_BRIDGE_SELECTION_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Event/Subscriber/SelectionSubscriber.h"
#include "Speckle/Event/Type/SelectionEvent.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to support sending model data to a Speckle server
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

		bool handle(const speckle::event::SelectionEvent& event) override;
	};
	
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SELECTION_BRIDGE
