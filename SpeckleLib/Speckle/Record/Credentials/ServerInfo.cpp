#include "Speckle/Record/Credentials/ServerInfo.h"

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
		nameID,
		companyID,
		versionID,
		contactID,
		descriptionID,
		frontEndID,
		urlID,
		migrationID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
		Identity{"company"},
		Identity{"version"},
		Identity{"adminContact"},
		Identity{"description"},
		Identity{"frontend2"},
		Identity{"url"},
		Identity{"migration"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ServerInfo::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
			{ fieldID[companyID], companyID, element },
			{ fieldID[versionID], versionID, element },
			{ fieldID[contactID], contactID, element },
			{ fieldID[descriptionID], descriptionID, element },
			{ fieldID[frontEndID], frontEndID, element },
			{ fieldID[urlID], urlID, element },
			{ fieldID[migrationID], migrationID, element },
		},
	}.withType(&typeid(ServerInfo)));
	return true;
} //ServerInfo::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ServerInfo::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ServerInfo))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			return std::make_unique<ValueWrap<String>>(m_name);
		case companyID:
			return std::make_unique<ValueWrap<String>>(m_company);
		case versionID:
			return std::make_unique<ValueWrap<String>>(m_version);
		case contactID:
			return std::make_unique<ValueWrap<String>>(m_adminContact);
		case descriptionID:
			return std::make_unique<ValueWrap<String>>(m_description);
		case frontEndID:
			return std::make_unique<ValueWrap<bool>>(m_frontend2);
		case urlID:
			return std::make_unique<ValueWrap<String>>(m_url);
		case migrationID:
			return std::make_unique<PackageWrap>(m_migration);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ServerInfo::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ServerInfo::setDefault() {
	m_name.clear();
	m_company.clear();
	m_version.clear();
	m_adminContact.clear();
	m_description.clear();
	m_frontend2 = false;
	m_url.clear();
} //ServerInfo::setDefault
