#include "Connector/Interface/Browser/Bridge/Send/SendBridge.h"
#include "Connector/Interface/Browser/Bridge/Send/GetSendFilters.h"
#include "Connector/Interface/Browser/Bridge/Send/GetSendSettings.h"
#include "Connector/Interface/Browser/Bridge/Send/Send.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Speckle/Event/Type/ElementChangedEvent.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

using namespace speckle::database;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;
using namespace speckle::event;
using namespace active::serialise;

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
bool SendBridge::handle(const ElementChangedEvent& event) {
	
	auto eventType = event.getEventType();
	switch (eventType)
	{
		case ElementChangedEvent::EventType::Begin: {
			m_changedElements.clear();
		} break;
		case ElementChangedEvent::EventType::End: {
			auto modelCardDatabase = connector()->getModelCardDatabase();
			auto modelCards = modelCardDatabase->getCards();

			RecordIDList idList;
			for (const auto& card : modelCards)
				idList.push_back(card->getID());

			// TODO: search for expired modelcards

			if (idList.empty())
				return true;

			auto wrapped = std::make_unique<CargoHold<ContainerWrap<RecordIDList>, RecordIDList>>(std::move(idList));
			sendEvent("setModelsExpired", std::move(wrapped));

		} break;
		case ElementChangedEvent::EventType::Change:
		case ElementChangedEvent::EventType::Edit: {
			auto changedElement = event.getChangedElement();
			m_changedElements.push_back(changedElement);
		} break;
		default:
			break;
	}
	return true;
} //SendBridge::handle
