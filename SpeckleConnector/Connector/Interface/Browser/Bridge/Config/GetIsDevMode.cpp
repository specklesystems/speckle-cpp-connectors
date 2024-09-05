#include "Connector/Interface/Browser/Bridge/Config/GetIsDevMode.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Return type for retrieving the current configuration
	using WrappedValue = CargoHold<ValueWrap<bool>, bool>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetIsDevMode::GetIsDevMode() : BridgeMethod{"GetIsDevMode", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the DEBUG mode
 
	return: true for DEBUG, false otherwise
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetIsDevMode::run() const {

#if DEBUG
	bool value = true;
#else
	bool value = false;
#endif
	
	return std::make_unique<WrappedValue>(value);
} //GetIsDevMode::run
