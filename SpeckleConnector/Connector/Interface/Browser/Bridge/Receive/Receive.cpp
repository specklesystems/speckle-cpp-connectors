#include "Connector/Interface/Browser/Bridge/Receive/Receive.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Record/Collection/ProjectCollection.h"
#include "Connector/Record/Model/ReceiverModelCard.h"
#include "Speckle/Database/AccountDatabase.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Environment/Host.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Record/Credentials/Account.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Serialise/Detached/Storage/DetachedMemoryStore.h"
#include "Speckle/Utility/Exception.h"

using namespace active::serialise;
using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::element;
using namespace speckle::serialise;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Receive::Receive() : BridgeMethod{"Receive", [&](const ReceiveArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Receive a specified model
 
	modelCardID: The ID of the model card identifying the objects to receive
  --------------------------------------------------------------------*/
void Receive::run(const String& modelCardID) const {
} //Receive::run
