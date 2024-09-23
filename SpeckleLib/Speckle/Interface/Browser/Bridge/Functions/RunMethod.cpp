#include "Speckle/Interface/Browser/Bridge/Functions/RunMethod.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Null.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Interface/Browser/Bridge/Functions/ErrorReport.h"
#include "Speckle/Utility/Exception.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <MessageLoopExecutor.hpp>
#endif

using namespace active::serialise;
using namespace speckle::interfac::browser;
using namespace speckle::interfac::browser::bridge;
using namespace speckle::serialise::jsbase;
using namespace speckle::utility;

namespace {

		///Default error message used when no description is available from the exception
	const char* defaultError = "An unidentified exception was thrown";
	
	/*!
	 Format the error message from an exception thrown during bridge method execution
	 @param message The exception error message (what happened)
	 @param argument The method argument (also carries the method name etc)
	 @return The formatted message
	 */
	String formattedErrorMessage(const String& message, const BridgeArgument& argument) {
		return "Exception thrown by the Speckle connector executing method '" + argument.getMethodName() + "': \"" + message + "\"";
	} //formattedErrorMessage

	
	/*!
	 Execute a bridge method using a specified argument
	 @param bridge The bridge
	 @param method The bridge method to execute
	 @param argument The method argument
	 */
	void executeMethod(BrowserBridge& bridge, Functional<>& method, BridgeArgument& argument) {
		std::optional<ErrorReport> errorReport;
			//If the argument validation failed (during deserialisation) then we simply fill in the exception report without running the method
		if (argument.hasError())
			errorReport = ErrorReport{argument.errorMessage().value_or(String{})};
		else {
			try {
					//Execute the method with the supplied argument
				auto result = method.execute(argument);
					//Cache the result in the bridge as required (when we have a request ID and a non-void result)
				if (!argument.getRequestID().empty()) {
					if (!result)
						result = std::make_unique<Null>();	//Callers need a null response even if the function has no return value
					bridge.cacheResult(std::move(result), argument.getRequestID());
				}
				return;
			} catch(std::runtime_error e) {
					//NB: This will capture the response from both Speckle and low-level system/runtime error exceptions
				errorReport = ErrorReport{e.what(), typeid(e).name()};
			} catch(...) {}
		}
			//This point is only reached in error conditions - ensure a response is provided if it hasn't been establishd already
		if (!errorReport)
			errorReport = ErrorReport{defaultError};
		errorReport->message = formattedErrorMessage(errorReport->message, argument);
			//Cache the error report to be sent back to the JS caller against the request ID
		bridge.cacheResult(std::make_unique<CargoHold<PackageWrap, ErrorReport>>(std::make_unique<ErrorReport>(*errorReport)), argument.getRequestID());
	} //executeMethod
	
	
	///NB: The following is an Archicad-specific method for executing a function asynchronously within the app event queue
	///Implement for other platforms as required
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
		RunBrowserMethod(BrowserBridge& bridge, Functional<>& method, std::shared_ptr<BridgeArgument> argument) :
				m_bridge{bridge}, m_method{method}, m_argument{argument} {}

		/*!
		 Execute the function and (when required) cache the result or an error report
		 */
		void Run() override {
			if (m_argument)
				executeMethod(m_bridge, m_method, *m_argument);
		}
		
	private:
			///The parent browser bridge
		BrowserBridge& m_bridge;
			///The bridge method to execute
		Functional<>& m_method;
			///The method argument
		std::shared_ptr<BridgeArgument> m_argument;
	};
#endif
	
}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
RunMethod::RunMethod() : JSFunction{"RunMethod", [&](auto args) {
		runMethod(args);
	}} {
} //RunMethod::RunMethod


/*--------------------------------------------------------------------
	Run a specified bridge method
 
	arguments: The method arguments
  --------------------------------------------------------------------*/
void RunMethod::runMethod(BridgeArgumentWrap& argument) const {
		//Confirm argument and function validity
	if (!argument || !hasBridge())
		return;
	auto method = getBridge()->getMethod(argument.getMethodName());
	if (method == nullptr)
		return;
		//NB: Implement the equivalent run scheduling for other platforms as required
#ifdef ARCHICAD
	GS::MessageLoopExecutor executor;
	try {
		executor.Execute(new RunBrowserMethod(*getBridge(), *method, argument.get()));
	}
	catch (...) {}
#endif
} //RunMethod::runMethod
