#include "Speckle/Environment/Host.h"

#include "Active/Utility/Memory.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/SpeckleResource.h"

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
#ifdef ServerMainVers_2700
	if ((ACAPI_Window_GetCurrentWindow(&windowInfo) == NoError) && (windowInfo.typeID == APIWind_3DModelID))
#else
	if ((ACAPI_Database(APIDb_GetCurrentWindowID, &windowInfo) == NoError) && (windowInfo.typeID == APIWind_3DModelID))
#endif
		return true;
	if (isSelectionOnly)
#ifdef ServerMainVers_2700
		return (ACAPI_View_ShowSelectionIn3D() == NoError);
#else
		return (ACAPI_Automate(APIDo_ShowSelectionIn3DID) == NoError);
#endif
#ifdef ServerMainVers_2700
	return (ACAPI_View_ShowAllIn3D() == NoError);
#else
	return (ACAPI_Automate(APIDo_ShowAllIn3DID) == NoError);
#endif
#endif //ARCHICAD
} //Host::makeModelViewActive


/*--------------------------------------------------------------------
	Zoom the active view to fit the content bounds
 
	isSelectionOnly: True to zoom to fit just the current selection
 --------------------------------------------------------------------*/
void Host::zoomToFit(bool isSelectionOnly) const {
#ifdef ARCHICAD
	if (isSelectionOnly)
#ifdef ServerMainVers_2700
		ACAPI_View_ZoomToSelected();
#else
		ACAPI_Automate(APIDo_ZoomToSelectedID);
#endif
	else
#ifdef ServerMainVers_2700
		ACAPI_View_Zoom();
#else
		ACAPI_Automate(APIDo_ZoomID);
#endif
#endif //ARCHICAD
} //Host::zoomToFit


/*--------------------------------------------------------------------
	Display an alert dialog
 
	message: The alert message
 --------------------------------------------------------------------*/
void Host::displayAlert(const String& message) const {
#ifdef ARCHICAD
	DGAlert(DG_INFORMATION, addon()->getLocalString(titleStringLib, notifyDialogTitleID), message, "",
			addon()->getLocalString(titleStringLib, okButtonTitleID));
#endif
} //Host::displayAlert


/*--------------------------------------------------------------------
	Display a confirmation dialog (prompting the user to pick one of two options)
 
	question: The question text, e.g. "Do you wish to continue?"
	positiveOption: The positive option text (nullopt =  "Yes")
	negativeOption: The negative option text (nullopt = "No")
 
	return: True if the user picked the positive option
 --------------------------------------------------------------------*/
bool Host::displayConfirmation(const speckle::utility::String& question,
							   const speckle::utility::String::Option positiveOption,
							   const speckle::utility::String::Option negativeOption) const {
#ifdef ARCHICAD
	String positivePrompt{positiveOption.value_or(addon()->getLocalString(titleStringLib, positiveResponseTitleID))},
			negativePrompt{positiveOption.value_or(addon()->getLocalString(titleStringLib, negativeResponseTitleID))};
	return (DGAlert(DG_WARNING, addon()->getLocalString(titleStringLib, confirmDialogTitleID), question, String{}, positivePrompt, negativePrompt) == 1);
#endif
} //Host::displayConfirmation


/*--------------------------------------------------------------------
	Get an object representing the parent process/application
 
	return: The active application instance
 --------------------------------------------------------------------*/
speckle::environment::Host* speckle::environment::host() {
	return &m_hostInstance;
} //speckle::environment::host
