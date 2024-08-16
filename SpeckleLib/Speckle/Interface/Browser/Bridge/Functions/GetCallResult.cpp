#include "Speckle/Interface/Browser/Bridge/Functions/GetCallResult.h"

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <MessageLoopExecutor.hpp>
#endif

using namespace active::serialise;
using namespace speckle::serialise::jsbase;
using namespace speckle::interface::browser;
using namespace speckle::interface::browser::bridge;

/*--------------------------------------------------------------------
	Constructor
 
	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
GetCallResult::GetCallResult(BrowserBridge& bridge) : m_bridge{bridge},
		JSFunction{"GetCallResult", [&](auto args) { return runMethod(args); }} {
} //GetCallResult::GetCallResult


/*--------------------------------------------------------------------
	Run a specified bridge method
 
	arguments: The method arguments
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetCallResult::runMethod(JSBridgeArgumentWrap& argument) const {
		//Confirm argument and function validity
	if (!argument || (argument.getObjectName() != m_bridge.getName()))
		return nullptr;
	return m_bridge.releaseResult(argument.getRequestID());
} //GetCallResult::runMethod
