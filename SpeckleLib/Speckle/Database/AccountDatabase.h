#ifndef SPECKLE_DATABASE_ACCOUNT_DBASE
#define SPECKLE_DATABASE_ACCOUNT_DBASE

#include "Active/File/Path.h"
#include "Speckle/Record/Credentials/Account.h"

namespace speckle::database {
		
	/*!
		A base class for an addon
	*/
	class AccountDatabase {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param path Path to the database file
		 */
		AccountDatabase(const active::file::Path& path);
		AccountDatabase(const AccountDatabase&) = delete;
		/*!
		 Destructor
		 */
		~AccountDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get all accounts
		 @return All the accounts
		 */
		active::container::Vector<record::cred::Account> getAccounts() const;
		
		// MARK: - Functions (mutating)
				
	private:
		class Store;
			///Accounts database storage
		std::unique_ptr<Store> m_store;
	};
	
}

#endif	//SPECKLE_DATABASE_ACCOUNT_DBASE
