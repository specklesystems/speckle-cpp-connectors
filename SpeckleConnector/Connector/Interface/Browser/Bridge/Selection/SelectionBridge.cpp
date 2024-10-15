#include "Connector/Interface/Browser/Bridge/Selection/SelectionBridge.h"

#include "Connector/Interface/Browser/Bridge/Selection/GetSelection.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SelectionBridge::SelectionBridge() : BrowserBridge{"selectionBinding"} {
		//Add bridge methods
	addMethod<GetSelection>();
} //SelectionBridge::SelectionBridge
