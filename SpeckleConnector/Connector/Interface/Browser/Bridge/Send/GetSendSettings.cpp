#include "Connector/Interface/Browser/Bridge/Send/GetSendSettings.h"

#include "Active/Container/Vector.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Connector/Record/Model/CardSetting.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<ContainerWrap<Vector<CardSetting>, PackageWrap>, Vector<CardSetting>>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetSendSettings::GetSendSettings() : BridgeMethod{"GetSendSettings", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the send filters
 
	return: The send filters
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetSendSettings::run() const {
	auto filters = std::make_unique<Vector<CardSetting>>();
	return std::make_unique<WrappedValue>(std::move(filters));
} //GetSendSettings::run
