#include "Active/Database/Storage/SQLiteEngine.h"
#include "Active/Database/Storage/Storage.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Setting/ValueSetting.h"
#include "Active/Setting/Values/StringValue.h"
#include "Speckle/Database/AccountDatabase.h"

using namespace active::container;
using namespace active::database;
using namespace active::serialise::json;
using namespace active::setting;
using namespace speckle::record::cred;
using namespace speckle::database;
using namespace speckle::utility;

namespace {
	
	enum Fields {
		hashID = 0,
		contentID,
	};
	
	const char* accountsDBaseName = "accounts";
	const char* accountsTableName = "objects";
	const char* hashFieldName = "hash";
	const char* contentFieldName = "content";

}

namespace speckle::database {

		///Accounts database engine declaration
	using AccountsEngine = SQLiteEngine<Account, Account, JSONTransport, active::utility::String, active::utility::String>;

		///Accounts database storage declaration
	class AccountDatabase::Store : public active::database::Storage<speckle::record::cred::Account, active::serialise::json::JSONTransport,
			active::utility::String, active::utility::String, active::utility::String, active::utility::String> {
		using base = active::database::Storage<speckle::record::cred::Account, active::serialise::json::JSONTransport,
				active::utility::String, active::utility::String, active::utility::String, active::utility::String>;
		using base::base;
	};
	
}

/*--------------------------------------------------------------------
	Constructor
 
	path: Path to the database file
  --------------------------------------------------------------------*/
AccountDatabase::AccountDatabase(const active::file::Path& path) {
	m_store = std::make_unique<Store>(
			//Engine
		std::make_unique<AccountsEngine>(path,
				//Schema
			 DBaseSchema{active::utility::String{accountsDBaseName},
					//Tables
				{
					{
						accountsTableName, Fields::hashID, Fields::contentID, {
							ValueSetting{StringValue{}, hashFieldName},
							ValueSetting{StringValue{}, contentFieldName},
						}
					}
				}
			}
		)
	);
} //AccountDatabase::AccountDatabase


/*--------------------------------------------------------------------
	Copy constructor
 
	source: The object to copy
  --------------------------------------------------------------------*/
AccountDatabase::~AccountDatabase() {}


/*--------------------------------------------------------------------
	Get all accounts
 
	return: All the accounts
  --------------------------------------------------------------------*/
Vector<Account> AccountDatabase::getAccounts() const {
	return m_store->getObjects();
} //AccountDatabase::getAccounts
