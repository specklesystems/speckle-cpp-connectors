#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"

#include "Active/Serialise/CargoHold.h"
#include "Connector/Connector.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Base/Arg/DocumentInfo.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Utility/Guid.h"

using namespace active::container;
using namespace active::serialise;
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
	if (auto project = connector()->getActiveProject().lock(); project) {
		auto info = project->getInfo();
		docInfo->name = info.name;
		if (info.path)
			docInfo->location = *info.path;
		if (auto cardDatabase = connector()->getModelCardDatabase(); cardDatabase != nullptr)
			docInfo->ID = cardDatabase->getStoreID();
	}
	return std::make_unique<WrappedValue>(std::move(docInfo));
} //GetDocumentInfo::run
