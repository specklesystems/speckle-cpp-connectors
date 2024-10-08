#include "Connector/Interface/Browser/Bridge/Send/SendBridge.h"

#include "Connector/Interface/Browser/Bridge/Send/GetSendFilters.h"
#include "Connector/Interface/Browser/Bridge/Send/Send.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SendBridge::SendBridge() : BrowserBridge{"sendBinding"} {
		//Add bridge methods
	addMethod<GetSendFilters>();
	addMethod<Send>();
} //SendBridge::SendBridge
