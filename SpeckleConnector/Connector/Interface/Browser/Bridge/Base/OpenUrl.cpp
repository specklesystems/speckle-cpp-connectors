#include "Connector/Interface/Browser/Bridge/Base/OpenUrl.h"

using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
OpenUrl::OpenUrl() : BridgeMethod{"OpenUrl", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Opens an url
 
	url: The URL to open
  --------------------------------------------------------------------*/
void OpenUrl::run(const String& url) const {
	std::string command = "start " + url;
	system(command.c_str());
} //OpenUrl::run
