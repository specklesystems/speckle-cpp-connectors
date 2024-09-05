#include "Speckle/Record/Credentials/UserInfo.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Speckle/Utility/Guid.h"

#include <array>

using namespace active::serialise;
using namespace speckle::record::cred;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		idID,
		nameID,
		emailID,
		companyID,
		avatarID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"id"},
		Identity{"name"},
		Identity{"email"},
		Identity{"company"},
		Identity{"avatar"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool UserInfo::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[idID], idID, element },
			{ fieldID[nameID], nameID, element },
			{ fieldID[emailID], emailID, element },
			{ fieldID[companyID], companyID, element },
			{ fieldID[avatarID], avatarID, element },
		},
	}.withType(&typeid(UserInfo)));
	return true;
} //UserInfo::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique UserInfo::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(UserInfo))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case idID:
			return std::make_unique<ValueWrap<String>>(m_id);
		case nameID:
			return std::make_unique<ValueWrap<String>>(m_name);
		case emailID:
			return std::make_unique<ValueWrap<String>>(m_email);
		case companyID:
			return std::make_unique<ValueWrap<String>>(m_company);
		case avatarID:
			return std::make_unique<ValueWrap<String>>(m_avatar);
		default:
			return nullptr;	//Requested an unknown index
	}
} //UserInfo::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void UserInfo::setDefault() {
	m_id.clear();
	m_name.clear();
	m_company.clear();
	m_email.clear();
	m_avatar.clear();
} //UserInfo::setDefault
