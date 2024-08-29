#include "Connector/Interface/Browser/Bridge/Config/UpdateConfig.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/PackageWrap.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
UpdateConfig::UpdateConfig() : BridgeMethod{"UpdateConfig", [&](UpdateArgs args) {
		run(args.value);
}} {}


/*--------------------------------------------------------------------
	Update the configuration settings
 
	config: The new settings
  --------------------------------------------------------------------*/
void UpdateConfig::run(const ConnectorConfig& config) const {
	///TODO: Store the active configuration settings here
} //UpdateConfig::run
