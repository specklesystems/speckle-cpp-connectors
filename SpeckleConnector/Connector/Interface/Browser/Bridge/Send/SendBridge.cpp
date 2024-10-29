#include "Connector/Interface/Browser/Bridge/Send/SendBridge.h"
#include "Connector/Interface/Browser/Bridge/Send/GetSendFilters.h"
#include "Connector/Interface/Browser/Bridge/Send/GetSendSettings.h"
#include "Connector/Interface/Browser/Bridge/Send/Send.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Speckle/Event/Type/ElementEvent.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Connector/Record/Model/SenderModelCard.h"
#include "Connector/Record/Model/Filter/SendFilter.h"

using namespace speckle::database;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;
using namespace speckle::event;
using namespace active::serialise;
using namespace connector::record;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SendBridge::SendBridge() : BrowserBridge{"sendBinding"} {
		//Add bridge methods
	addMethod<GetSendFilters>();
	addMethod<GetSendSettings>();
	addMethod<Send>();

	// POC: do we have a better place to attach observer to elements?
#ifdef ARCHICAD
	auto project = connector()->getActiveProject().lock();
	if (!project) {
		// TODO: is this OK? should this throw?
		return;
	}

	auto elementDatabase = project->getElementDatabase();
	auto elements = elementDatabase->getElements();
	ElementIDList elementIds;
	for (const auto& id : elementIds) {
		ACAPI_Element_AttachObserver(id);
	}
#endif
} //SendBridge::SendBridge

/*--------------------------------------------------------------------
	Handle an element change

	event: The selection event

	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool SendBridge::handle(const ElementEvent& event) {
	
	auto eventType = event.getEventType();
	switch (eventType)
	{
		case ElementEvent::EventType::Begin: {
			m_changedElements.clear();
		} break;
		case ElementEvent::EventType::End: {
			auto modelCardDatabase = connector()->getModelCardDatabase();
			auto modelCards = modelCardDatabase->getCards();

			// POC: this is probably not efficient, test and review it
			RecordIDList expiredModelCardIds;
			for (const auto& modelCard : modelCards) {
				if (auto senderCard = dynamic_cast<SenderModelCard*>(modelCard.get())) {
					auto modelCardSelection = senderCard->getFilter().getElementIDs();

					for (const auto& elemId : modelCardSelection) {
						if (std::find(m_changedElements.begin(), m_changedElements.end(), elemId) != m_changedElements.end()) {
							expiredModelCardIds.push_back(modelCard->getID());
							break;
						}
					}
				}
			}

			if (!expiredModelCardIds.empty()) {
				auto wrapped = std::make_unique<CargoHold<ContainerWrap<RecordIDList>, RecordIDList>>(std::move(expiredModelCardIds));
				sendEvent("setModelsExpired", std::move(wrapped));
			}
		} break;
		case ElementEvent::EventType::Change:
		case ElementEvent::EventType::Edit:
		case ElementEvent::EventType::Delete: {
			auto changedElement = event.getChangedElement();
			m_changedElements.push_back(changedElement);
		} break;
		default:
		  break;
	}
	return true;
} //SendBridge::handle
