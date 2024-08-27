#include "Connector/Interface/Browser/Bridge/Test/GoAway.h"

using namespace connector::interfac::browser::bridge;

#include <iostream>

/*--------------------------------------------------------------------
	Default constructor
 --------------------------------------------------------------------*/
GoAway::GoAway() : JSBridgeMethod{"GoAway", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Write a message to the console
  --------------------------------------------------------------------*/
void GoAway::run() const {
	std::cout << "Okay, going away.";
} //GoAway::run
