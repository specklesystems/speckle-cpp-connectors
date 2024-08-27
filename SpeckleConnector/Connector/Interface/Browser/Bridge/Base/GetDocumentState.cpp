#include "Connector/Interface/Browser/Bridge/Base/GetDocumentState.h"

#include "Active/Serialise/CargoHold.h"
#include "Connector/Database/Model/Card/ModelCardDatabase.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace connector::database;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<PackageWrap, ModelCardDatabase>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetDocumentState::GetDocumentState() : JSBridgeMethod{"GetDocumentState", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the document info
 
	return: The document info
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetDocumentState::run() const {
		///TODO: Retrieve the model card database from connector()->getModelCards() in future (when implemented)
	ModelCardDatabase modelCards;	//This is just a temp so something can be sent back to the JS for the interim
	return std::make_unique<WrappedValue>(modelCards);
} //GetDocumentState::run
