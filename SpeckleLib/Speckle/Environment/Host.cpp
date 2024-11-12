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
#ifdef ServerMainVers_2600
	if ((ACAPI_Window_GetCurrentWindow(&windowInfo) == NoError) && (windowInfo.typeID == APIWind_3DModelID))
#else
	if ((ACAPI_Database(APIDb_GetCurrentWindowID, &windowInfo) == NoError) && (windowInfo.typeID == APIWind_3DModelID))
#endif
		return true;
	if (isSelectionOnly)
#ifdef ServerMainVers_2600
		return (ACAPI_View_ShowSelectionIn3D() == NoError);
#else
		return (ACAPI_Automate(APIDo_ShowSelectionIn3DID) == NoError);
#endif
#ifdef ServerMainVers_2600
	return (ACAPI_View_ShowAllIn3D() == NoError);
#else
	return (ACAPI_Automate(APIDo_ShowAllIn3DID) == NoError);
#endif
#endif //ARCHICAD
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
