#include "Connector/Interface/Browser/Bridge/Selection/GetSelection.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Selection/Arg/SelectionInfo.h"

#include "Connector/Connector.h"
#include "Connector/Record/Collection/ProjectCollection.h"

#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Element/Element.h"
using namespace speckle::record::element;

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;
using namespace connector::record;

namespace {

	///Return type for retrieving the current configuration
	using WrappedValue = CargoHold<PackageWrap, SelectionInfo>;

}
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

	auto project = connector()->getActiveProject().lock();
	if (!project) {
		// TODO
	}
		
	//Build a collection from the selected elements
	auto collection = std::make_unique<ProjectCollection>(project);
	auto elementDatabase = project->getElementDatabase();
	auto selected = elementDatabase->getSelection();

	active::utility::String summary(selected.size());
	summary += " objects selected.";

	selectionInfo->summary = summary;

	for (const auto& link : selected) {
		if (auto element = elementDatabase->getElement(link); element) {
			collection->addElement(*element);
			selectionInfo->selectedElementIds.push_back(element->getBIMID());
		}
	}

	return std::make_unique<WrappedValue>(std::move(selectionInfo));
} //GetSelection::run
