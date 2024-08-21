#include "Connector/Interface/Browser/Bridge/Config/GetConfig.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = CargoHold<PackageWrap, ConnectorConfig>;

}

/*--------------------------------------------------------------------
	Constructor
 
	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
GetConfig::GetConfig() : JSBridgeMethod{"GetConfig", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get an argument instance for the function (used to deserialise/unpack incoming arguments)
 
	return: An argument instance
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetConfig::getArgument() const { return nullptr; }


/*--------------------------------------------------------------------
	Get the configuration settings
 
	return: The settings
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetConfig::run(void) const {
	ConnectorConfig config;
	///TODO: Get the accounts here - returning an empty array for testing only
	return std::make_unique<WrappedValue>(config);
} //GetConfig::run
