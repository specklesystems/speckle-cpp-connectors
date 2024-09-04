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
		ServerInfo(const utility::String& name, const utility::String& company, const utility::String& version, const utility::String& contact,
				   const utility::String& description, const utility::String& url, bool isFrontEnd, const ServerMigration& migration) :
				m_name{name}, m_company{company}, m_version{version}, m_adminContact{contact}, m_description{description},
				m_url{url}, m_frontend2{isFrontEnd}, m_migration{migration} {}
		ServerInfo(const ServerInfo&) = default;

		// MARK: - Functions (const)
		
		/*!
		 Get the server name
		 @return The server name
		 */
		const utility::String& getName() const { return m_name; }
		
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
			///Server name
		utility::String m_name;
			///Company name
		utility::String m_company;
			///Server version
		utility::String m_version;
			///Admin contact email
		utility::String m_adminContact;
			///Server description
		utility::String m_description;
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
		ServerMigration m_migration;
	};

}

#endif	//SPECKLE_RECORD_CRED_SERVER_INFO
