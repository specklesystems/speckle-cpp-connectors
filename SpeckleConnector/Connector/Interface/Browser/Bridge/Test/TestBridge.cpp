#include "Connector/Interface/Browser/Bridge/Test/TestBridge.h"

#include "Connector/Interface/Browser/Bridge/Test/GetComplexType.h"
#include "Connector/Interface/Browser/Bridge/Test/GoAway.h"
#include "Connector/Interface/Browser/Bridge/Test/SayHi.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
TestBridge::TestBridge() : BrowserBridge{"baseBinding"} {
		//Add bridge methods
	addMethod<GetComplexType>();
	addMethod<GoAway>();
	addMethod<SayHi>();
} //TestBridge::TestBridge
