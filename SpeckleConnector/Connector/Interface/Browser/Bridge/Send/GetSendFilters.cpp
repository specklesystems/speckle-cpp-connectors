#include "Connector/Interface/Browser/Bridge/Send/GetSendFilters.h"

#include "Active/Container/Vector.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Record/Model/Filter/ArchicadEverythingFilter.h"
#include "Connector/Record/Model/Filter/FilterMover.h"
#include "Connector/Record/Model/Filter/ArchicadSelectionFilter.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<ContainerWrap<Vector<SendFilter>, FilterMover>, Vector<SendFilter>>;

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
	auto filters = std::make_unique<Vector<SendFilter>>();
	//filters.emplace_back(ArchicadEverythingFilter{});	//TODO: Implement as required
	filters->emplace_back(ArchicadSelectionFilter{});
	return std::make_unique<WrappedValue>(std::move(filters));
} //GetSendFilters::run
