#ifndef SPECKLE_RECORD_CRED_SERVER_INFO
#define SPECKLE_RECORD_CRED_SERVER_INFO

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Record/Credentials/ServerMigration.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::cred {
	
	/*!
	 Account server info
	 */
	class ServerInfo : public active::serialise::Package {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ServerInfo() {}
		/*!
		 Constructor
		 @param name The server name
		 @param company The company name
		 @param version The server version
		 @param contact Admin contact email
		 @param description The server description
		 @param url The server URL
		 @param isFrontEnd ?
		 @param migration Server migration record
		 */
		ServerInfo(const utility::String& name, const utility::String::Option company = std::nullopt,
				   const utility::String::Option version = std::nullopt, const utility::String::Option contact = std::nullopt,
				   const utility::String::Option description = std::nullopt, const utility::String url = {},
				   bool isFrontEnd = false, std::unique_ptr<ServerMigration> migration = nullptr) :
				m_name{name}, m_company{company}, m_version{version}, m_adminContact{contact}, m_description{description},
				m_url{url}, m_frontend2{isFrontEnd}, m_migration{std::move(migration)} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ServerInfo(const ServerInfo& source) { copy(source); }
		/*!
		 Destructor
		 */
		~ServerInfo() {}

		// MARK: - Operators
		
		/*!
		 Assignment operator
		 @param source The object to copy
		 */
		ServerInfo& operator=(const ServerInfo& source) { copy(source); return *this; }

		// MARK: - Functions (const)
		
		/*!
		 Get the server name
		 @return The server name
		 */
		const utility::String& getName() const { return m_name; }
		/*!
		 Get the company name
		 @return The company name
		 */
		const utility::String::Option& getCompany() const { return m_company; }
		/*!
		 Get the version
		 @return The version
		 */
		const utility::String::Option& getVersion() const { return m_version; }
		/*!
		 Get the admin contact email
		 @return The admin contact email
		 */
		const utility::String::Option& getAdminContact() const { return m_adminContact; }
		/*!
		 Get the description
		 @return The description
		 */
		const utility::String::Option& getDescription() const { return m_description; }
		/*!
		 Determine if ?
		 @return ?
		 */
		bool isFrontEnd() const { return m_frontend2; }
		/*!
		 Get the URL
		 @return The URL
		 */
		const utility::String& getURL() const { return m_url; }
		/*!
		 Get the migration history
		 @return The migration history (nullptr = no history)
		 */
		const ServerMigration* getMigration() const { return m_migration.get(); }
		
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
		
	private:
		/*!
		 Copy from another object
		 @param source The object to copy
		 */
		void copy(const ServerInfo& source);
		
			///Server name
		utility::String m_name;
			///Company name
		utility::String::Option m_company;
			///Server version
		utility::String::Option m_version;
			///Admin contact email
		utility::String::Option m_adminContact;
			///Server description
		utility::String::Option m_description;
		/*!
		 This field is not returned from the GQL API, it should be populated after construction from the response headers.
		 See "Speckle.Core.Credentials.AccountManager"
		 */
		bool m_frontend2 = false;
		/*!
		 Server URL
		 This field is not returned from the GQL API, it should be populated after construction.
		 See "Speckle.Core.Credentials.AccountManager"
		 */
		utility::String m_url;
			///Server migration record
		std::unique_ptr<ServerMigration> m_migration;
	};

}

#endif	//SPECKLE_RECORD_CRED_SERVER_INFO
