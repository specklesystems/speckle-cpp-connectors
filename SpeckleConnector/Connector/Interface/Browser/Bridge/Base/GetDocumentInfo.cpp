#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"

#include "Active/Serialise/CargoHold.h"
#include "Connector/Connector.h"
#include "Connector/Interface/Browser/Bridge/Base/Arg/DocumentInfo.h"
#include "Speckle/Environment/Project.h"

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
		///TODO: Get the document info here - returning mocked values for now
	DocumentInfo docInfo;
	if (auto project = connector()->getActiveProject().lock(); project) {
		auto info = project->getInfo();
		docInfo.name = info.name;
		if (info.path)
			docInfo.location = *info.path;
			//TODO: No suitable project ID is currently available
	}
	return std::make_unique<WrappedValue>(docInfo);
} //GetDocumentInfo::run
