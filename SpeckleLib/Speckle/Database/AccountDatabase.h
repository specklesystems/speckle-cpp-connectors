#ifndef SPECKLE_DATABASE_ACCOUNT_DBASE
#define SPECKLE_DATABASE_ACCOUNT_DBASE

#include "Active/File/Path.h"
#include "Speckle/Record/Credentials/Account.h"

namespace speckle::record::cred {
	class Account;
}

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
		 Get a specified account. NB: The server URL is provided as a fallback for the search if the specified accountID is not found
		 @param accountID The account ID (the primary search field)
		 @param serverURL The server URL (a fallback search field if the account ID does not exist)
		 @return The requested account (nullptr on failure)
		 */
		std::unique_ptr<record::cred::Account> getAccount(const speckle::utility::String& accountID, const speckle::utility::String& serverURL) const;
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
