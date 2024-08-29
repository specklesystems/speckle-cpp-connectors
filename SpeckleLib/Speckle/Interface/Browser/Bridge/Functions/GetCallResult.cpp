#include "Speckle/Interface/Browser/Bridge/Functions/GetCallResult.h"

#include "Active/Serialise/Item/Wrapper/ItemWrap.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

#include <utility>

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <MessageLoopExecutor.hpp>
#endif

using namespace active::serialise;
using namespace speckle::serialise::jsbase;
using namespace speckle::interfac::browser;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
GetCallResult::GetCallResult() : JSFunction{"GetCallResult", [&](auto args) {
		return getResult(args);
	}} {
} //GetCallResult::GetCallResult


/*--------------------------------------------------------------------
	Get the result for a specified call
 
	argument: The method arguments specifying the target bridge and requestID
 
	return: The requested result (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<WrappedResultArg> GetCallResult::getResult(WrappedResultArg& argument) const {
	if (!hasBridge())
		return nullptr;
		//Retrieve the requested result
	using namespace json;
	auto result = getBridge()->releaseResult(argument);
	auto item = dynamic_cast<Cargo*>(result.get());
	if (!item)
		return nullptr;
	String jsonOutput;
	JSONTransport().send(std::forward<Cargo&&>(*item), Identity{}, jsonOutput);
	return std::make_unique<WrappedResultArg>(jsonOutput);
} //GetCallResult::getResult
