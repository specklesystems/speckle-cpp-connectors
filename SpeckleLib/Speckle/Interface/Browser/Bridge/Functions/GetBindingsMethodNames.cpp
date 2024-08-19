#include "Speckle/Interface/Browser/Bridge/Functions/GetBindingsMethodNames.h"

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

using namespace active::serialise;
using namespace speckle::serialise::jsbase;
using namespace speckle::interfac::browser;
using namespace speckle::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Constructor
 
	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
GetBindingsMethodNames::GetBindingsMethodNames(BrowserBridge& bridge) : m_bridge{bridge},
		JSFunction{"GetBindingsMethodNames", [&]() {
			return getMethodNames();
		}} {
} //GetBindingsMethodNames::GetBindingsMethodNames


/*--------------------------------------------------------------------
	Get the names of the methods supported by the parent browser
 
	return: The supported method names
  --------------------------------------------------------------------*/
std::unique_ptr<WrappedValue> GetBindingsMethodNames::getMethodNames() const {
	return std::make_unique<WrappedValue>(m_bridge.getMethodNames());
} //GetBindingsMethodNames::getMethodNames
