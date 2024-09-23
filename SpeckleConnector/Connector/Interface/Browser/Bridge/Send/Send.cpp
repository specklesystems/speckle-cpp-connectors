#include "Connector/Interface/Browser/Bridge/Send/Send.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Speckle/Utility/Exception.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Send::Send() : BridgeMethod{"Send", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the madel to send
  --------------------------------------------------------------------*/
void Send::run(const String& modelCardID) const {
		///TODO: Find and send selected elements - the following is a placeholder
	throw Exception{"No objects were found to convert. Please update your publish filter!"};
} //Send::run
