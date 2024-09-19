#include "Connector/Record/Model/ModelCard.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

#include <array>

using namespace active::container;
using namespace active::serialise;
using namespace connector::record;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		modelID,
		projectID,
		accountID,
		serverURLID,
		settingsID,
		expiredID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"modelId"},
		Identity{"projectId"},
		Identity{"accountId"},
		Identity{"serverURL"},
		Identity{"settings"},
		Identity{"expired"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ModelCard::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[modelID], modelID, element },
			{ fieldID[projectID], projectID, element },
			{ fieldID[accountID], accountID, element },
			{ fieldID[serverURLID], serverURLID, element },
			{ fieldID[settingsID], settingsID, element },
			{ fieldID[expiredID], expiredID, element },
		},
	}.withType(&typeid(ModelCard)));
		//This class has a unique serialisation tag for the record ID - override the base class ID
	inventory.merge(Inventory{
		{
			{ Identity{"modelCardId"}, active::database::record::FieldIndex::idIndex, element },
		},
	}.withType(&typeid(active::database::Record<>)));
	return true;
} //ModelCard::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ModelCard::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ModelCard))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case modelID:
			return std::make_unique<ValueWrap<String>>(m_modelID);
		case projectID:
			return std::make_unique<ValueWrap<String>>(m_projectID);
		case accountID:
			return std::make_unique<ValueWrap<String>>(m_accountID);
		case serverURLID:
			return std::make_unique<ValueWrap<String>>(m_serverURL);
		case settingsID:
			return std::make_unique<ContainerWrap<Vector<CardSetting>>>(m_settings);
		case expiredID:
			return std::make_unique<ValueWrap<bool>>(m_isExpired);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ModelCard::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ModelCard::setDefault() {
	base::setDefault();
	m_modelID.clear();
	m_projectID.clear();
	m_accountID.clear();
	m_serverURL.clear();
	m_settings.clear();
} //ModelCard::setDefault
