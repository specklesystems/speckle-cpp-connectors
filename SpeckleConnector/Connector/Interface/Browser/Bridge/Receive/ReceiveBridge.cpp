#include "Connector/Interface/Browser/Bridge/Receive/ReceiveBridge.h"
#include "Connector/Interface/Browser/Bridge/Receive/Receive.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Speckle/Event/Type/ElementEvent.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Connector/Record/Model/ReceiverModelCard.h"

using namespace speckle::database;
using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;
using namespace speckle::event;
using namespace active::serialise;
using namespace connector::record;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ReceiveBridge::ReceiveBridge() : BrowserBridge{"receiveBinding"} {
		//Add bridge methods
	addMethod<Receive>();
} //ReceiveBridge::ReceiveBridge
