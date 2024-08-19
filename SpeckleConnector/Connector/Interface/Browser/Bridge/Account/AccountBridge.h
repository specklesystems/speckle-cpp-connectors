#ifndef SPECKLE_INTERFACE_BROWSER_ACCOUNT_BRIDGE
#define SPECKLE_INTERFACE_BROWSER_ACCOUNT_BRIDGE

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A browser bridge to provide account details to the caller
	*/
	class AccountBridge : public speckle::interfac::browser::bridge::BrowserBridge {
	public:
		
		// MARK: - Types
		
		using base = speckle::interfac::browser::bridge::BrowserBridge;
		
		// MARK: - Constructors
		
		using base::base;
		/*!
		 Default constructor
		 */
		AccountBridge();
	};
	
}

#endif	//SPECKLE_INTERFACE_BROWSER_ACCOUNT_BRIDGE
