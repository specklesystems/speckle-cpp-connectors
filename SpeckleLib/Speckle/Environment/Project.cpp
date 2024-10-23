#include "Speckle/Environment/Project.h"

#include "Speckle/Database/BIMAttributeDatabase.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Database/BIMGroupDatabase.h"
#include "Speckle/Database/BIMPropertyDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/SpeckleResource.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::utility;

namespace {
	
}

/*--------------------------------------------------------------------
	Constructor (NB: this object is assumed to be the active instance)
 
	identity: Optional name/ID for the subscriber
  --------------------------------------------------------------------*/
Project::Project() {
	m_attribute = std::make_unique<BIMAttributeDatabase>();
	m_element = std::make_unique<BIMElementDatabase>();
	m_group = std::make_unique<BIMGroupDatabase>();
	m_property = std::make_unique<BIMPropertyDatabase>();
} //Project::Project


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Project::~Project() {
} //Project::~Project


/*--------------------------------------------------------------------
	Get information about the project
 
	return: Project information
  --------------------------------------------------------------------*/
Project::Info Project::getInfo() const {
		//Start with an untitled project - this will be replaced if a saved project is active
	Info result{addon()->getLocalString(titleStringLib, untitledProjectID)};
#ifdef ARCHICAD
	API_ProjectInfo	projectInfo;
	if (ACAPI_ProjectOperation_Project(&projectInfo) == NoError) {
		if ((projectInfo.projectName != nullptr) && !projectInfo.projectName->IsEmpty())
			result.name = *projectInfo.projectName;
		result.isShared = projectInfo.teamwork;
		if ((projectInfo.projectPath != nullptr) && !projectInfo.projectPath->IsEmpty())
			result.path = String{*projectInfo.projectPath};
		else if (projectInfo.teamwork) {
			if (projectInfo.location_team != nullptr) {
				GS::UniString path;
				if (projectInfo.location_team->ToPath(&path) == NoError)
					result.path = String{path};
			}
		} else if (projectInfo.location != nullptr) {
			GS::UniString path;
			if (projectInfo.location->ToPath(&path) == NoError)
				result.path = String{path};
		}
	}
#endif
	return result;
} //Project::getInfo
