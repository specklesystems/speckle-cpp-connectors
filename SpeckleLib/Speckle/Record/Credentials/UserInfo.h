#ifndef SPECKLE_RECORD_CRED_USER_INFO
#define SPECKLE_RECORD_CRED_USER_INFO

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::cred {
	
	/*!
	 User info
	 */
	class UserInfo : public active::serialise::Package {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param ID The user ID
		 @param name The user name
		 @param email The user email
		 @param company The company name
		 @param avatar ?
		 */
		UserInfo(const utility::String& ID, const utility::String& name, const utility::String& email, const utility::String& company,
				 const utility::String& avatar) :
				m_id{ID}, m_name{name}, m_email{email}, m_company{company}, m_avatar{avatar} {}
		UserInfo(const UserInfo&) = default;

		// MARK: - Functions (const)
		
		/*!
		 Get the user ID
		 @return The user ID
		 */
		const utility::String& getID() const { return m_id; }
		/*!
		 Get the user name
		 @return The user name
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
			///User ID
		utility::String m_id;
			///User name
		utility::String m_name;
			///User email
		utility::String m_email;
			///Compsny name
		utility::String m_company;
			///Avatar?
		utility::String m_avatar;
	};

}

#endif	//SPECKLE_RECORD_CRED_USER_INFO
