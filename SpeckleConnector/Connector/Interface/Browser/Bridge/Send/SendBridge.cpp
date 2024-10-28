#include "Connector/Interface/Browser/Bridge/Send/SendBridge.h"
#include "Connector/Interface/Browser/Bridge/Send/GetSendFilters.h"
#include "Connector/Interface/Browser/Bridge/Send/GetSendSettings.h"
#include "Connector/Interface/Browser/Bridge/Send/Send.h"
#include "Speckle/Event/Type/ElementChangedEvent.h"

using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SendBridge::SendBridge() : BrowserBridge{"sendBinding"} {
		//Add bridge methods
	addMethod<GetSendFilters>();
	addMethod<GetSendSettings>();
	addMethod<Send>();
} //SendBridge::SendBridge

/*--------------------------------------------------------------------
	Handle an element change

	event: The selection event

	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool SendBridge::handle(const speckle::event::ElementChangedEvent& event) {
	auto changedElementIDs = event.getChangedElementIDs();

	for (const auto& id : changedElementIDs) {

	}

	//auto wrapped = std::make_unique<CargoHold<PackageWrap, SelectionInfo>>(std::move(selectionInfo));
	//sendEvent("setSelection", std::move(wrapped));
	return true;
} //SendBridge::handle
