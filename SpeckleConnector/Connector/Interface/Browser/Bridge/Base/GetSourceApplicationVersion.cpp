#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationVersion.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace active::setting;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Return type for retrieving the host application version
	using WrappedValue = CargoHold<ValueWrap<String>, String>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetSourceApplicationVersion::GetSourceApplicationVersion() : BridgeMethod{"GetSourceApplicationVersion", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the host application version
 
	return: The application version
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetSourceApplicationVersion::run() const {
		//Implement other platforms as required
#ifdef ARCHICAD
	API_ServerApplicationInfo appInfo{};
	ACAPI_GetReleaseNumber(&appInfo);
		//NB: Assuming only the main version number is required - can be extended if necessary
	String result{appInfo.mainVersion};
	return std::make_unique<WrappedValue>(result);
#endif
} //GetSourceApplicationVersion::run
