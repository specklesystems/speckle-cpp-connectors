#include "Connector/Interface/Browser/Bridge/Base/BaseBridge.h"

#include "Connector/Interface/Browser/Bridge/Base/AddModel.h"
#include "Connector/Interface/Browser/Bridge/Base/GetConnectorVersion.h"
#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"
#include "Connector/Interface/Browser/Bridge/Base/GetDocumentState.h"
#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationName.h"
#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationVersion.h"
#include "Connector/Interface/Browser/Bridge/Base/RemoveModel.h"
#include "Connector/Interface/Browser/Bridge/Base/UpdateModel.h"
#include "Connector/Interface/Browser/Bridge/Base/HighlightModel.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
BaseBridge::BaseBridge() : BrowserBridge{"baseBinding"} {
		//Add bridge methods
	addMethod<AddModel>();
	addMethod<GetConnectorVersion>();
	addMethod<GetDocumentInfo>();
	addMethod<GetDocumentState>();
	addMethod<GetSourceApplicationName>();
	addMethod<GetSourceApplicationVersion>();
	addMethod<RemoveModel>();
	addMethod<UpdateModel>();
	addMethod<HighlightModel>();
} //BaseBridge::BaseBridge
