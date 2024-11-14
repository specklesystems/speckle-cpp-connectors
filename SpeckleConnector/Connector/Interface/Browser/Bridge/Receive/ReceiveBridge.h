#ifndef CONNECTOR_INTERFACE_BRIDGE_RECEIVE_BRIDGE
#define CONNECTOR_INTERFACE_BRIDGE_RECEIVE_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to support receiving model data from a Speckle server
	*/
	class ReceiveBridge : public speckle::interfac::browser::bridge::BrowserBridge {
	public:
		
		// MARK: - Types
		
		using base = speckle::interfac::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		using base::base;
		/*!
		 Default constructor
		 */
		ReceiveBridge();
	};
	
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_RECEIVE_BRIDGE
