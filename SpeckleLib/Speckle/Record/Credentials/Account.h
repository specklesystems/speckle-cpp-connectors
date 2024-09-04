#ifndef SPECKLE_RECORD_CRED_ACCOUNT
#define SPECKLE_RECORD_CRED_ACCOUNT

#include "Active/Setting/SettingList.h"
#include "Active/Database/Content/Record.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::cred {
	
	/*!
	 User account record class
	 */
	class Account : public active::database::Record<speckle::utility::String, speckle::utility::String, speckle::utility::String> {
	public:

		// MARK: - Types
		
		using base = active::database::Record<speckle::utility::String, speckle::utility::String, speckle::utility::String>;
			///Unique pointer
		using Unique = std::unique_ptr<Account>;
			///Shared pointer
		using Shared = std::shared_ptr<Account>;
			///Optional
		using Option = std::optional<Account>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 @param ID The account record ID
		 */
		Account(const speckle::utility::String& ID = speckle::utility::String{});
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Account(const Account& source) : base{source} {}
		/*!
			Object cloning
			@return A clone of this object
		*/
		Account* clonePtr() const override { return new Account{*this}; }


		// MARK: - Functions (const)
		
		// MARK: - Functions (mutating)
		
		// MARK: - Serialisation
		
		/*!
			Fill an inventory with the package items
			@param inventory The inventory to receive the package items
			@return True if the package has added items to the inventory
		*/
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
			Get the specified cargo
			@param item The inventory item to retrieve
			@return The requested cargo (nullptr on failure)
		*/
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		/*!
			Set to the default package content
		*/
		void setDefault() override;
		/*!
			Validate the cargo data
			@return True if the data has been validated
		*/
		bool validate() override;
		
	private:
			///Authorisation token
		speckle::utility::String m_token;
			///Authorisation refresh token
		speckle::utility::String m_refreshToken;
			///The account to have as the default selection in any UI
		bool m_isDefault = false;
			///True if the account is online
		bool m_isOnline = true;
			///Account server info
		//std::unique_ptr<ServerInfo> m_serverInfo;
			///Account server info
		//std::unique_ptr<UserInfo> m_userInfo;
	};

}

#endif	//SPECKLE_RECORD_CRED_ACCOUNT
