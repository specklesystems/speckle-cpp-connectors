#ifndef SPECKLE_INTERFACE_BROWSER_ACCOUNT_BRIDGE
#define SPECKLE_INTERFACE_BROWSER_ACCOUNT_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

namespace connector::interface::browser::bridge {
	
	/*!
	 A browser bridge to provide account details to the caller
	*/
	class AccountBridge : public speckle::interface::browser::bridge::BrowserBridge {
	public:
		
		// MARK: - Types
		
		using base = speckle::interface::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		AccountBridge();
	};
	
}

#endif	//SPECKLE_INTERFACE_BROWSER_ACCOUNT_BRIDGE
