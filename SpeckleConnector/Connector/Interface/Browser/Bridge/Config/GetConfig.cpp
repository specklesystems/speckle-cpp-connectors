#include "Connector/Interface/Browser/Bridge/Config/GetConfig.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Return type for retrieving the current configuration
	using WrappedValue = CargoHold<PackageWrap, ConnectorConfig>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetConfig::GetConfig() : JSBridgeMethod{"GetConfig", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the configuration settings
 
	return: The settings
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetConfig::run(void) const {
	ConnectorConfig config;
	///TODO: Get the accounts here - returning an empty array for testing only
	return std::make_unique<WrappedValue>(config);
} //GetConfig::run
