#include "Connector/Tool/ElementHighlighter.h"

#include "Active/Database/Transaction.h"
#include "Active/Event/Event.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Event/ConnectorEventID.h"
#include "Speckle/Database/BIMAttributeDatabase.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Environment/Host.h"
#include "Speckle/Record/Filter/ElementVisibilityCollector.h"
#include "Speckle/Utility/Guid.h"

using namespace active::database;
using namespace active::event;
using namespace active::setting;
using namespace connector;
using namespace connector::environment;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record;
using namespace speckle::utility;

namespace {
	
	/*!
	 Transaction to make a selection of layers visible
	 */
	class ShowLayers : public Transaction {
	public:
		/*!
		 Constructor
		 @param project The target project
		 @param layers The layers in the project to be made visible
		 */
		ShowLayers(Project::Shared project, ElementVisibilityCollector::Layers& layers) :
				Transaction{connector::connector()->getLocalString(titleString, showLayersID)}, m_project{project}, m_layers{layers} {}
		
	protected:
		
		//MARK: - Functions (App management)
		
		/*!
		 Perform the transaction. Database writes can be performed. NB: this function is not called if the prepare phase was unsuccessful)
		 @return True if the transaction was successfully performed
		 */
		bool perform() override {
			auto attributeDatabase = m_project->getAttributeDatabase();
			for (auto& layer : m_layers) {
				if (layer.second.isHidden()) {
					layer.second.setHidden(false);
					attributeDatabase->write(layer.second);
				}
			}
			return true;
		}
		
	private:
			///The target project for showing layers
		Project::Shared m_project;
			///The layers to be made visible
		ElementVisibilityCollector::Layers& m_layers;
	};
	
}

/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription ElementHighlighter::subscription() const {
	return Subscription{setElementHighlight};
} //ElementHighlighter::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ElementHighlighter::receive(const active::event::Event& event) {
		//Collect the IDs of elements to be highlighted
	ValueSetting* elementIDs = nullptr;
	if (elementIDs = event.findValue(recordLinks); elementIDs == nullptr)
		return false;
	BIMLinkList elementSelection;
	for (const auto& value : *elementIDs)
		if (Guid guid{value->operator active::utility::Guid()}; guid)
			elementSelection.emplace_back(guid);
	if (elementSelection.empty())
		return false;
	auto project = connector()->getActiveProject().lock();
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return false;
	auto elementDatabase = project->getElementDatabase();
		//Collect the layers assigned to the model card elements
	ElementVisibilityCollector collector;
	BIMElementDatabase::Filter filter = [&collector](const speckle::record::element::Element& elem) { return collector(elem); };
	elementDatabase->findElements(&filter, elementSelection);
		//If any collected layers are hidden, the useer is prompted to show them (otherwise they may see nothing happen when a model card is clicked)
	for (const auto& layer : collector.getLayers()) {
		if (layer.second.isHidden()) {
				//If a hidden layer is found, ask the user before taking any action
			if (host()->displayConfirmation(addon()->getLocalString(notifyString, showHiddenLayersID))) {
					//Run a transaction to ensure all layers are visible
				ShowLayers showLayers{project, collector.getLayers()};
				connector()->makeTransaction(showLayers);
			}
			break;
		}
	}
	host()->makeModelViewActive();
	elementDatabase->clearSelection();
	elementDatabase->setSelection(elementSelection);
	host()->zoomToFit(true);
	return false;
} //ElementHighlighter::receive
