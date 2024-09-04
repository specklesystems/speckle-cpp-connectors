#include "Active/Database/Storage/SQLiteEngine.h"
#include "Active/Setting/ValueSetting.h"
#include "Active/Setting/Values/StringValue.h"
#include "Speckle/Database/AccountDBase.h"

using namespace active::database;
using namespace active::serialise::json;
using namespace active::setting;
using namespace speckle::record::cred;
using namespace speckle::database;
using namespace speckle::utility;

using AccountsEngine = SQLiteEngine<Account, Account, JSONTransport, active::utility::String, active::utility::String>;

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
//	template<typename Obj, typename ObjWrapper, typename Transport, typename DocID = utility::Guid, typename ObjID = utility::Guid>

/*--------------------------------------------------------------------
	Constructor
 
	path: Path to the database file
  --------------------------------------------------------------------*/
AccountDBase::AccountDBase(const active::file::Path& path) : base{
		std::make_unique<AccountsEngine>(
			path, DBaseSchema{
				accountsDBaseName, {
					{
						accountsTableName, Fields::hashID, Fields::contentID, {
							ValueSetting{StringValue{}, hashFieldName},
							ValueSetting{StringValue{}, contentFieldName},
						}
					}
				}
			}
		)
	} {
	
} //AccountDBase::AccountDBase
