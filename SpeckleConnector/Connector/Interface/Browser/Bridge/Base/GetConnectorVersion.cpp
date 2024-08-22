#include "Connector/Interface/Browser/Bridge/Base/GetConnectorVersion.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Connector/Version.h"

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
GetConnectorVersion::GetConnectorVersion() : JSBridgeMethod{"GetConnectorVersion", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the host application version
 
	return: The application version
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetConnectorVersion::run() const {
		//Implement other platforms as required
#ifdef ARCHICAD
	API_ServerApplicationInfo appInfo{};
	ACAPI_GetReleaseNumber(&appInfo);
		//NB: Assuming only the main version number is required - can be extended if necessary
	String result{active::utility::String{versionMajor} + "." + active::utility::String{versionMinor} + "." +
			active::utility::String{versionPatch}};
	return std::make_unique<WrappedValue>(result);
#endif
} //GetConnectorVersion::run
