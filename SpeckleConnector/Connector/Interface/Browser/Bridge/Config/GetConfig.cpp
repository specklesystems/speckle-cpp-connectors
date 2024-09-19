#include "Connector/Interface/Browser/Bridge/Config/GetConfig.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"

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
GetConfig::GetConfig() : BridgeMethod{"GetConfig", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the configuration settings
 
	return: The settings
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetConfig::run() const {
	ConnectorConfig config;
		///TODO: Get the data from a local SQLite database
	return std::make_unique<WrappedValue>(config);
} //GetConfig::run
