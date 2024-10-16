#include "Connector/Interface/Browser/Bridge/Selection/GetSelection.h"
#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Selection/Arg/SelectionInfo.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetSelection::GetSelection() : BridgeMethod{"GetSelection", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the model to send
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetSelection::run() const {
	auto selectionInfo = std::make_unique<SelectionInfo>();
	return std::make_unique<CargoHold<PackageWrap, SelectionInfo>>(std::move(selectionInfo));
} //GetSelection::run
