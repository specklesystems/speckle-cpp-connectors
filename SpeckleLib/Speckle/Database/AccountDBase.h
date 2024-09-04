#ifndef SPECKLE_DATABASE_ACCOUNT_DBASE
#define SPECKLE_DATABASE_ACCOUNT_DBASE

#include "Active/Database/Storage/SQLiteEngine.h"
#include "Active/Database/Storage/Storage.h"
#include "Active/File/Path.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Speckle/Record/Credentials/Account.h"
#include "Speckle/Utility/String.h"

#include <type_traits>

namespace speckle::database {
		
	/*!
		A base class for an addon
	*/
	class AccountDBase : public active::database::Storage<speckle::record::cred::Account, active::serialise::json::JSONTransport,
			active::utility::String, active::utility::String, active::utility::String, active::utility::String> {
	public:

		// MARK: - Types
		
		using base = active::database::Storage<speckle::record::cred::Account, active::serialise::json::JSONTransport,
				active::utility::String, active::utility::String, active::utility::String, active::utility::String>;

		using type_identity_t = std::type_identity<active::database::Storage<speckle::record::cred::Account, active::serialise::json::JSONTransport,
				active::utility::String, active::utility::String, active::utility::String, active::utility::String>>::type;
				
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param path Path to the database file
		 */
		AccountDBase(const active::file::Path& path);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		AccountDBase(const AccountDBase& source) : base{source} {}
		/*!
		 Destructor
		 */
		~AccountDBase();
		
		// MARK: - Functions (const)
		
		
		// MARK: - Functions (mutating)
		
	};
	
}

#endif	//SPECKLE_DATABASE_ACCOUNT_DBASE
