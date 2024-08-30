#include "Connector/Interface/Browser/Bridge/Test/SayHi.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

using namespace active::serialise;
using namespace active::setting;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Return type for the test message
	using WrappedValue = CargoHold<ValueWrap<String>, String>;

}

/*--------------------------------------------------------------------
	Default constructor
 --------------------------------------------------------------------*/
SayHi::SayHi() : BridgeMethod{"SayHi", [&](SayHiWrapper arg) {
		return run(arg);
}} {}


/*--------------------------------------------------------------------
	Return a message based on sample data
 
	arg: The sample data
 
	return: The message based on sample data
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> SayHi::run(SayHiArg arg) const {
	String result, baseGreeting{(arg.isHello ? "Hello" : "Hi") + arg.name + "!\n"};
	for (auto i = arg.count; i--; )
		result += baseGreeting;
	return std::make_unique<WrappedValue>(result);
} //SayHi::run
