#include "Connector/Interface/Browser/Bridge/Base/GetDocumentInfo.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Connector/Interface/Browser/Bridge/Base/DocumentInfo.h"

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
GetDocumentInfo::GetDocumentInfo() : JSBridgeMethod{"GetDocumentInfo", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the document info
 
	return: The document info
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetDocumentInfo::run(void) const {
		///TODO: Get the document info here - returning mocked values for now
	DocumentInfo docInfo{"Somewhere", "Something", String{active::utility::Guid{true}.operator active::utility::String()}};
	return std::make_unique<WrappedValue>(docInfo);
} //GetDocumentInfo::run
