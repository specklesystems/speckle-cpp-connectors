#include "Connector/Interface/Browser/Bridge/Test/GetComplexType.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Test/Arg/ComplexType.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Return type for retrieving the current configuration
	using WrappedValue = CargoHold<PackageWrap, ComplexType>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetComplexType::GetComplexType() : BridgeMethod{"GetComplexType", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the required object type
 
	return: The required object
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetComplexType::run() const {
	ComplexType object;
	return std::make_unique<WrappedValue>(object);
} //GetComplexType::run
