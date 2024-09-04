#include "Speckle/Record/Credentials/Account.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::record::cred;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		tokenID,
		refreshTokenID,
		isDefaultID,
		isOnlineID,
		serverInfoID,
		userInfoID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"token"},
		Identity{"refreshToken"},
		Identity{"isDefault"},
		Identity{"isOnline"},
		Identity{"serverInfo"},
		Identity{"userInfo"},
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Account::Account(const String& ID) : base{ID} {
	//TODO: Complete
} //Account::Account


/*--------------------------------------------------------------------
	Fill an inventory with the package items
	@param inventory The inventory to receive the package items
	@return True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Account::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[tokenID], tokenID, element },
			{ fieldID[refreshTokenID], refreshTokenID, element },
			{ fieldID[isDefaultID], isDefaultID, element },
			{ fieldID[isOnlineID], isOnlineID, element },
		},
	}.withType(&typeid(Account)));
	return base::fillInventory(inventory);
} //Account::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
	@param item The inventory item to retrieve
	@return The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Account::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Account))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case tokenID:
			return std::make_unique<ValueWrap<String>>(m_token);
		case refreshTokenID:
			return std::make_unique<ValueWrap<String>>(m_refreshToken);
		case isDefaultID:
			return std::make_unique<ValueWrap<bool>>(m_isDefault);
		case isOnlineID:
			return std::make_unique<ValueWrap<bool>>(m_isOnline);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Account::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Account::setDefault() {
	m_token.clear();
	m_refreshToken.clear();
	m_isDefault = false;
	m_isOnline = true;
} //Account::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool Account::validate() {
		//TODO: Fail conditions to be determined
	return true;
} //Account::validate
