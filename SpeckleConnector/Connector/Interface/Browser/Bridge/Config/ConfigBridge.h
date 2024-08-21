#ifndef CONNECTOR_INTERFACE_BRIDGE_CONFIG_BRIDGE
#define CONNECTOR_INTERFACE_BRIDGE_CONFIG_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to provide configuration settings
	*/
	class ConfigBridge : public speckle::interfac::browser::bridge::BrowserBridge {
	public:
		
		// MARK: - Types
		
		using base = speckle::interfac::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		using base::base;
		/*!
		 Default constructor
		 */
		ConfigBridge();
	};
	
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_CONFIG_BRIDGE
