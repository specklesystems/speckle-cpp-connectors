#include "Connector/Record/Model/SenderModelCard.h"

#include "Connector/Record/Model/Filter/SendFilter.h"

#include <array>

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		sendFilterID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"sendFilter"},
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SenderModelCard::SenderModelCard() {
	
} //SenderModelCard::SenderModelCard


/*--------------------------------------------------------------------
	Constructor
 
	filter: The filter applied when the model was sent
  --------------------------------------------------------------------*/
SenderModelCard::SenderModelCard(const SendFilter& filter) {
	
} //SenderModelCard::SenderModelCard


/*--------------------------------------------------------------------
	Copy constructor
 
	source: The object to copy
  --------------------------------------------------------------------*/
SenderModelCard::SenderModelCard(const SenderModelCard& source) {
	
} //SenderModelCard::SenderModelCard


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
SenderModelCard::~SenderModelCard() {
	
} //SenderModelCard::~SenderModelCard


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SenderModelCard::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
/*	inventory.merge(Inventory{
		{
			{ fieldID[modelID], modelID, element },
			{ fieldID[projectID], projectID, element },
			{ fieldID[accountID], accountID, element },
			{ fieldID[serverURLID], serverURLID, element },
			{ fieldID[settingsID], settingsID, element },
		},
	}.withType(&typeid(SenderModelCard)));
	return base::fillInventory(inventory);*/
} //SenderModelCard::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SenderModelCard::getCargo(const Inventory::Item& item) const {
/*	if (item.ownerType != &typeid(SenderModelCard))
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
			return std::make_unique<ContainerWrap<Vector, CardSetting>>(m_settings);
		default:
			return nullptr;	//Requested an unknown index
	}*/
} //SenderModelCard::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void SenderModelCard::setDefault() {
/*	base::setDefault();
	m_modelID.clear();
	m_projectID.clear();
	m_accountID.clear();
	m_serverURL.clear();
	m_settings.clear();*/
} //SenderModelCard::setDefault
