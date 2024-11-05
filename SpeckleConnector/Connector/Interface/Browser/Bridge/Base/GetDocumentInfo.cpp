#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"

#include "Active/Serialise/CargoHold.h"
#include "Connector/Connector.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Base/Arg/DocumentInfo.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Utility/Guid.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::environment;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<PackageWrap, DocumentInfo>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetDocumentInfo::GetDocumentInfo() : BridgeMethod{"GetDocumentInfo", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the document info
 
	return: The document info
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetDocumentInfo::run() const {
	auto docInfo = std::make_unique<DocumentInfo>();
	auto project = connector()->getActiveProject().lock();
	auto connectorProject = dynamic_cast<ConnectorProject*>(project.get());
	if (!connectorProject)
		return nullptr;
	auto info = connectorProject->getInfo();
	docInfo->name = info.name;
	if (info.path)
		docInfo->location = *info.path;
	if (auto cardDatabase = connectorProject->getModelCardDatabase(); cardDatabase != nullptr)
		docInfo->ID = cardDatabase->getStoreID();
	docInfo->ID = Guid{true}.operator String();
	return std::make_unique<WrappedValue>(std::move(docInfo));
} //GetDocumentInfo::run
