#include "Connector/Interface/Browser/Bridge/Send/Send.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Send::Send() : BridgeMethod{"Send", [&](UpdateArgs args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the madel to send
  --------------------------------------------------------------------*/
void Send::run(const String& modelCardID) const {
	///TODO: Send the requested model
} //Send::run
