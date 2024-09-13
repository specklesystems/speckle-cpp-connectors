#include "Connector/Interface/Browser/Bridge/Send/GetSendFilters.h"

#include "Active/Container/Vector.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Record/Model/Filter/SendFilter.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<ContainerWrap<Vector, SendFilter>, Vector<SendFilter>>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetSendFilters::GetSendFilters() : BridgeMethod{"GetSendFilters", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the send filters
 
	return: The send filters
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetSendFilters::run() const {
	Vector<SendFilter> filters;
		///TODO: Get real filters
	return std::make_unique<WrappedValue>(filters);
} //GetSendFilters::run
