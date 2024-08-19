#include "Connector/Interface/Browser/Bridge/Account/AccountBridge.h"

#include "Connector/Interface/Browser/Bridge/Account/GetAccounts.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
AccountBridge::AccountBridge() : BrowserBridge{"accountsBinding"} {
		//Add bridge methods
	addMethod<GetAccounts>();
} //AccountBridge::AccountBridge
