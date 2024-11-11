#include "Connector/Interface/Browser/Bridge/Test/GoAway.h"

#include "Speckle/Environment/Platform.h"

using namespace connector::interfac::browser::bridge;
using namespace speckle::environment;

/*--------------------------------------------------------------------
	Default constructor
 --------------------------------------------------------------------*/
GoAway::GoAway() : BridgeMethod{"GoAway", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Write a message to the console
  --------------------------------------------------------------------*/
void GoAway::run() const {
	platform()->writeToConsole("Okay, going away.");
} //GoAway::run
