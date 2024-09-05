#include "Connector/Interface/Browser/Bridge/Config/ConfigBridge.h"

#include "Connector/Interface/Browser/Bridge/Config/GetConfig.h"
#include "Connector/Interface/Browser/Bridge/Config/UpdateConfig.h"
#include "Connector/Interface/Browser/Bridge/Config/GetIsDevMode.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ConfigBridge::ConfigBridge() : BrowserBridge{"configBinding"} {
		//Add bridge methods
	addMethod<GetConfig>();
	addMethod<UpdateConfig>();
	addMethod<GetIsDevMode>();
} //ConfigBridge::ConfigBridge
