#include "Active/Database/Storage/SQLite/SQLiteEngine.h"
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
	
		//Account field indices
	enum Fields {
		hashID = 0,
		contentID,
	};
	
		///Internal name of the accounts dbase
	const char* accountsDBaseName = "accounts";
		///Accounts table name
	const char* accountsTableName = "objects";
		///Hash field name
	const char* hashFieldName = "hash";
		///Content field name
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
		//Create accounts database storage (with schema)
	m_store = std::make_unique<Store>(
			//Engine
		std::make_unique<AccountsEngine>(path,
				//Schema
			 DBaseSchema{active::utility::String{accountsDBaseName},
					//Tables
				{
						//Account table
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
	Destructor
  --------------------------------------------------------------------*/
AccountDatabase::~AccountDatabase() {}


/*--------------------------------------------------------------------
	Get all accounts
 
	return: All the accounts
  --------------------------------------------------------------------*/
Vector<Account> AccountDatabase::getAccounts() const {
	return m_store->getObjects();
} //AccountDatabase::getAccounts
