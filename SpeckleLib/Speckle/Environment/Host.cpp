#include "Speckle/Environment/Host.h"

#ifdef ARCHICAD
#include "DG.h"
#endif

using namespace speckle::utility;
using namespace speckle::environment;

namespace {
	
		//An object representing the active host
	Host m_hostInstance;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Host::Host() {
} //Host::Host


/*--------------------------------------------------------------------
	Display an alert dialog
 
	message: The alert message
 --------------------------------------------------------------------*/
void Host::displayAlert(const String& message) const {
#ifdef ARCHICAD
	DGAlert(DG_INFORMATION, "Notification", message.data(), "", "OK");
#endif
} //Host::displayAlert


/*--------------------------------------------------------------------
	Get an object representing the parent process/application
 
	return: The active application instance
 --------------------------------------------------------------------*/
speckle::environment::Host* speckle::environment::host() {
	return &m_hostInstance;
} //speckle::environment::host
