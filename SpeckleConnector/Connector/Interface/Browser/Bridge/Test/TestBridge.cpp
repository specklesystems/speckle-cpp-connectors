#include "Connector/Interface/Browser/Bridge/Test/TestBridge.h"

#include "Connector/Interface/Browser/Bridge/Test/GetComplexType.h"
#include "Connector/Interface/Browser/Bridge/Test/GoAway.h"
#include "Connector/Interface/Browser/Bridge/Test/SayHi.h"
#include "Connector/Interface/Browser/Bridge/Test/TriggerEvent.h"

using namespace connector::interfac::browser::bridge;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
TestBridge::TestBridge() : BrowserBridge{"testBinding"} {
		//Add bridge methods
	addMethod<GetComplexType>();
	addMethod<GoAway>();
	addMethod<SayHi>();
	addMethod<TriggerEvent>();
} //TestBridge::TestBridge
