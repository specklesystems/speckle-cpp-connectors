#include "Speckle/Interface/Browser/Bridge/Functions/GetBindingsMethodNames.h"

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

using namespace active::serialise;
using namespace speckle::serialise::jsbase;
using namespace speckle::interfac::browser;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Constructor

	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
GetBindingsMethodNames::GetBindingsMethodNames() : JSFunction{"GetBindingsMethodNames", [&]() {
		return getMethodNames();
}} {} //GetBindingsMethodNames::GetBindingsMethodNames


/*--------------------------------------------------------------------
	Get the names of the methods supported by the parent browser

	return: The supported method names
  --------------------------------------------------------------------*/
std::unique_ptr<WrappedValue> GetBindingsMethodNames::getMethodNames() const {
	return (hasBridge()) ? std::make_unique<WrappedValue>(getBridge()->getMethodNames()) : nullptr;
} //GetBindingsMethodNames::getMethodNames
