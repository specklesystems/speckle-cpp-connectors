#include "Connector/Interface/Browser/Bridge/Config/UpdateConfig.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/ConnectorConfig.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Constructor
 
	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
UpdateConfig::UpdateConfig() : JSBridgeMethod{"UpdateConfig", [&](UpdateArgs args) {
		run(args.value);
}} {}


/*--------------------------------------------------------------------
	Get an argument instance for the function (used to deserialise/unpack incoming arguments)
 
	return: An argument instance
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> UpdateConfig::getArgument() const { return std::make_unique<UpdateArgs>(); }


/*--------------------------------------------------------------------
	Get the configuration settings
 
	return: The new settings
  --------------------------------------------------------------------*/
void UpdateConfig::run(const ConnectorConfig& config) const {
	///TODO: Store the active configuration settings here
} //UpdateConfig::run
