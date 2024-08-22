#include "ConnectorResource.h"
#include "Connector/Connector.h"
#include "Connector/Database/Model/Card/ModelCardDatabase.h"
#include "Interface/ConnectorMenu.h"
#include "Interface/ConnectorPalette.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Utility/String.h"

using namespace active::environment;
using namespace connector;
using namespace connector::database;
using namespace speckle::environment;
using namespace speckle::utility;

namespace {
	
		///The Connector addon class
	class ConnectorInstance : public ConnectorAddon {
	public:
		ConnectorInstance(const String& name) : ConnectorAddon{name} {
			add(std::make_shared<ConnectorMenu>());
			add(std::make_shared<ConnectorPalette>());
		}
		
		// MARK: Functions (const)
		
		/*!
		 Get the model card database
		 @return The model card database
		 */
		const ModelCardDatabase* getModelCards() const override { return &m_modelCards; }
		
	private:
		ModelCardDatabase m_modelCards;
	};
	
		///The active addon instance
	std::unique_ptr<ConnectorAddon> m_addonInstance;
	
}

/*--------------------------------------------------------------------
	Constructor

 	name: The add-on name
 --------------------------------------------------------------------*/
ConnectorAddon::ConnectorAddon(const speckle::utility::String& name) : Addon{name} {
} //ConnectorAddon::ConnectorAddon


#ifdef ARCHICAD
/*!
	The following is the C interface for the plugin to Archicad as specified by the API
*/

#include "ACAPinc.h"
#include "APIdefs_Registration.h"

/*--------------------------------------------------------------------
	Confirm that the plugin is able to run in the current environment

 	envir: Information about the add-on environment
 
 	return: An add-on type identifier
 --------------------------------------------------------------------*/
API_AddonType __ACENV_CALL CheckEnvironment(API_EnvirParams* envir) {
	m_addonInstance = std::make_unique<ConnectorInstance>(String{});
		//Populate the addon environent info
	envir->addOnInfo.name = addon()->getLocalString(titleString, addonNameID);
	envir->addOnInfo.description = addon()->getLocalString(titleString, addonDescriptionID);
	addon()->setName(envir->addOnInfo.name);
	return app()->audit() ? APIAddon_Preload : APIAddon_Unknown;
} //CheckEnvironment


/*--------------------------------------------------------------------
	Register the add-on interface, e.g. menus, tools etc
 
 	return: The add-on status (NoError if status is nominal)
 --------------------------------------------------------------------*/
GSErrCode __ACENV_CALL RegisterInterface() {
	ACAPI_KeepInMemory(true);
	return app()->attach() ? NoError : Cancel;
} //RegisterInterface


/*--------------------------------------------------------------------
	Initialise the add-on when loaded into memory
 
 	return: The add-on status (NoError if status is nominal)
 --------------------------------------------------------------------*/
GSErrCode __ACENV_CALL Initialize() {
	ACAPI_KeepInMemory(true);
	return app()->start() ? NoError : Cancel;
} //Initialize


/*--------------------------------------------------------------------
	Prepare for the add-on to be unloaded, e.g. release resources
 
 	return: The add-on status (NoError if status is nominal)
 --------------------------------------------------------------------*/
GSErrCode __ACENV_CALL FreeData() {
	app()->stop();
		//Release the active addon instance before the AC memory manager etc is destroyed
	m_addonInstance.reset();
	return NoError;
} //FreeData
#endif //ARCHICAD
