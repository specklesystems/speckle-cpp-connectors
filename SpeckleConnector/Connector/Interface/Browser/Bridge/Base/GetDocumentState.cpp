#include "Connector/Interface/Browser/Bridge/Base/GetDocumentState.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Connector/Connector.h"
#include "Connector/Record/Model/ModelCard.h"
#include "Connector/Database/ModelCardDatabase.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace connector::database;
using namespace connector::record;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<ContainerWrap<Vector, ModelCard>, Vector<ModelCard>>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetDocumentState::GetDocumentState() : BridgeMethod{"GetDocumentState", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the document info
 
	return: The document info
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetDocumentState::run() const {
	if (auto modelCardDBase = connector()->getModelCardDatabase(); modelCardDBase != nullptr) {
		return modelCardDBase->wrapper();
	}
	return nullptr;
} //GetDocumentState::run
