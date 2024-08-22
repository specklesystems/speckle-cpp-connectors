#include "Connector/Interface/Browser/Bridge/Base/GetSourceApplicationName.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace active::setting;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Return type for retrieving the host application name
	using WrappedValue = CargoHold<ValueWrap<String>, String>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetSourceApplicationName::GetSourceApplicationName() : JSBridgeMethod{"GetSourceApplicationName", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the host application name
 
	return: The application name
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetSourceApplicationName::run(void) const {
		//Implement other platforms as required
#ifdef ARCHICAD
	String result{"Archicad"};
	return std::make_unique<WrappedValue>(result);
#endif
} //GetSourceApplicationName::run
