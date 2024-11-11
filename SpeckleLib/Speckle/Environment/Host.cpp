#include "Speckle/Environment/Host.h"
#include "Active/Utility/Memory.h"

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
	Switch to the model view
 
	isSelectionOnly: True to display just the selected elements
 
 	return: True if the model view is active
 --------------------------------------------------------------------*/
bool Host::makeModelViewActive(bool isSelectionOnly) const {
#ifdef ARCHICAD
	API_WindowInfo windowInfo;
	active::utility::Memory::erase(windowInfo);
	if ((ACAPI_Window_GetCurrentWindow(&windowInfo) == NoError) && (windowInfo.typeID == APIWind_3DModelID))
		return true;
	if (isSelectionOnly)
		return (ACAPI_View_ShowSelectionIn3D() == NoError);
	return (ACAPI_View_ShowAllIn3D() == NoError);
#endif
} //Host::makeModelViewActive


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
