#include "Speckle/Interface/Browser/Bridge/Functions/RunMethod.h"

#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <MessageLoopExecutor.hpp>
#endif

using namespace active::serialise;
using namespace speckle::serialise::jsbase;
using namespace speckle::interfac::browser;
using namespace speckle::interfac::browser::bridge;

namespace {

#ifdef ARCHICAD
	/*!
	 Scheduled task to execute a browser bridge method in the Archicad event queue
	 */
	class RunBrowserMethod : public GS::Runnable {
	public:
		/*!
		 Constructor
		 @param bridge The parent browser bridge
		 @param method The bridge method to execute
		 @param argument The method argument
		 */
		RunBrowserMethod(BrowserBridge& bridge, Functional<>& method, std::shared_ptr<JSBridgeArgument> argument) :
				m_bridge{bridge}, m_method{method}, m_argument{argument} {}

		/*!
		 Execute the function and (when required) cache the result
		 */
		void Run() override {
			auto result = m_method.execute(*m_argument);
				//Cache the result in the bridge as required
			if (result && !m_argument->getRequestID().empty())
				m_bridge.cacheResult(std::move(result), m_argument->getRequestID());
		}
		
	private:
			///The parent browser bridge
		BrowserBridge& m_bridge;
			///The bridge method to execute
		Functional<>& m_method;
			///The method argument
		std::shared_ptr<JSBridgeArgument> m_argument;
	};
#endif
	
}

/*--------------------------------------------------------------------
	Constructor
 
	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
RunMethod::RunMethod(BrowserBridge& bridge) : m_bridge{bridge},
		JSFunction{"RunMethod", [&](auto args) { runMethod(args); }} {
} //RunMethod::RunMethod


/*--------------------------------------------------------------------
	Get an argument instance for the function (used to deserialise/unpack incoming arguments)
 
	return: An argument instance
  --------------------------------------------------------------------*/
std::unique_ptr<active::serialise::Cargo> RunMethod::getArgument() const {
	return std::make_unique<JSBridgeArgument>();
} //RunMethod::getArgument


/*--------------------------------------------------------------------
	Run a specified bridge method
 
	arguments: The method arguments
  --------------------------------------------------------------------*/
void RunMethod::runMethod(JSBridgeArgumentWrap& argument) const {
		//Confirm argument and function validity
	if (!argument)
		return;
	auto method = m_bridge.getMethod(argument.getMethodName());
	if (method == nullptr)
		return;
		//NB: Implement the equivalent run scheduling for other platforms as required
#ifdef ARCHICAD
	GS::MessageLoopExecutor executor;
	try {
		executor.Execute(new RunBrowserMethod(m_bridge, *method, argument.get()));
	}
	catch (...) {}
#endif
} //RunMethod::runMethod
