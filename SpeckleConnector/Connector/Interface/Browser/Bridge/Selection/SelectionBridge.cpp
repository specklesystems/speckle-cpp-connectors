#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Selection/SelectionBridge.h"
#include "Connector/Interface/Browser/Bridge/Selection/GetSelection.h"
#include "Connector/Interface/Browser/Bridge/Selection/Arg/SelectionInfo.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SelectionBridge::SelectionBridge() : BrowserBridge{"selectionBinding"} {
		//Add bridge methods
	addMethod<GetSelection>();
} //SelectionBridge::SelectionBridge


/*--------------------------------------------------------------------
	Handle a selection change
 
	event: The selection event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool SelectionBridge::handle(const speckle::event::SelectionEvent& event) {
	auto selectionInfo = std::make_unique<SelectionInfo>();
	auto wrapped =  std::make_unique<CargoHold<PackageWrap, SelectionInfo>>(std::move(selectionInfo));
	sendEvent("setSelection", std::move(wrapped));
	return true;
} //SelectionBridge::handle
