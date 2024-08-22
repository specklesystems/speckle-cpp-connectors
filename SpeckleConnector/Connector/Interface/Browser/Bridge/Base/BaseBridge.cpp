#include "Connector/Interface/Browser/Bridge/Base/BaseBridge.h"

#include "Connector/Interface/Browser/Bridge/Base/GetConnectorVersion.h"
#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"
#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationName.h"
#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationVersion.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
BaseBridge::BaseBridge() : BrowserBridge{"baseBinding"} {
		//Add bridge methods
	addMethod<GetConnectorVersion>();
	addMethod<GetDocumentInfo>();
	addMethod<GetSourceApplicationName>();
	addMethod<GetSourceApplicationVersion>();
} //BaseBridge::BaseBridge
