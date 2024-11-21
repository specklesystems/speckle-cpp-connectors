#include "Active/File/Directory.h"
#include "ConnectorResource.h"
#include "Connector/Connector.h"
#include "Connector/Environment/ConnectorProject.h"
#include "Connector/Interface/ConnectorMenu.h"
#include "Connector/Interface/ConnectorPalette.h"
#include "Connector/Tool/ElementHighlighter.h"
#include "Speckle/Database/AccountDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Utility/String.h"

#include <mutex>

#ifdef WINDOWS
	//NB: VS is ignoring template specialisations unless they are explicitly used in the top-level project
#include "Active/Setting/Values/GuidValue.h"
#include "Active/Setting/Values/TimeValue.h"
#include "Active/Setting/Values/StringValue.h"
using namespace active::setting;
#endif

using namespace active::file;
using namespace active::environment;
using namespace connector;
using namespace connector::environment;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::utility;

namespace {
	
		//The Speckle application data/support directory name
	const char* speckleDataDirName = "Speckle";
		//The account database name
	const char* accountDBaseName = "Accounts.db";
	
	
	/*!
	 Class for a concrete instance of an add-on
	 
	 This class is private to prevent ad-hoc construction of instances while fulfilling the requirements of the public interface. Essentially
	 this should behave as a singleton, representing the sole instance of the running add-on
	 */
	class ConnectorInstance : public ConnectorAddon {
	public:
		/*!
		 Constructor
		 @param name The connector add-on name
		 */
		ConnectorInstance(const String& name) : ConnectorAddon{name} {
				//Define the connector UI components
			add<ConnectorMenu>();
			add<ConnectorPalette>();
			add<ElementHighlighter>();
		}
		
		// MARK: Functions (const)
		
		/*!
		 Get the account database
		 @return The account database
		 */
		const AccountDatabase* getAccountDatabase() const override;
		
	protected:
		/*!
		 Make a new new project. Allows Addon subclasses to define a Project subclass with additional functions/databases
		 @return A new project instance
		 */
		virtual std::shared_ptr<Project> makeProject() const override {
			return std::dynamic_pointer_cast<Project>(std::make_shared<ConnectorProject>());
		}
		
	private:
			///The accounts database - always a single instance for the active user
		mutable std::unique_ptr<AccountDatabase> m_account;
			///Mutex to control access to the accounts database
		mutable std::mutex m_accountMutex;
	};
	
	
		///The active addon instance
	std::unique_ptr<ConnectorAddon> m_addonInstance;
	
	
	/*--------------------------------------------------------------------
		Get the speckle application data directory (creating if missing)
	 
		return: The application data directory (nullopt = missing and unable to create)
	 --------------------------------------------------------------------*/
	Directory::Option getAppDataDirectory() {
		auto appData = Directory::appData();
		if (!appData)
			return std::nullopt;
		return Directory{*appData, speckleDataDirName, true};
	} //getAppDataDirectory

#ifdef WINDOWS
		//NB: VS is ignoring template specialisations unless they are explicitly used in the top-level project
	void invokeSpecialisation() {
		StringValue stringValue;
		active::utility::String unusedString = stringValue;
		GuidValue guidValue;
		active::utility::Guid unusedGuid = guidValue;
		TimeValue timeValue;
		active::utility::Time unusedTime = timeValue;
	}
#endif
	
}

/*--------------------------------------------------------------------
	Constructor

 	name: The add-on name
 --------------------------------------------------------------------*/
ConnectorAddon::ConnectorAddon(const speckle::utility::String& name) : Addon{name} {
#ifdef WINDOWS
	invokeSpecialisation();
#endif
} //ConnectorAddon::ConnectorAddon


/*--------------------------------------------------------------------
	Get the account database
 
	return: The account database
 --------------------------------------------------------------------*/
const AccountDatabase* ConnectorInstance::getAccountDatabase() const {
	const std::lock_guard<std::mutex> lock{m_accountMutex};
	if (!m_account) {
		auto speckleDirectory = getAppDataDirectory();
		if (!speckleDirectory)
			return nullptr;
		m_account = std::make_unique<AccountDatabase>(speckleDirectory->getPath() / accountDBaseName);
	}
	return m_account.get();
} //ConnectorInstance::getAccounts


/*--------------------------------------------------------------------
Get an object representing the connector instance
@return The active connector instance (nullptr if no connector is running)
 --------------------------------------------------------------------*/
ConnectorAddon* connector::connector() {
	return m_addonInstance.get();
} //connector::connector



#ifdef ARCHICAD
/*!
	The following is the C interface for the plugin to Archicad as specified by the API
*/

#include <ACAPinc.h>
#include <APIdefs_Registration.h>

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
