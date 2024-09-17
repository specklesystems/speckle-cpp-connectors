#include "Connector/Record/Model/CardSetting.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Item/Wrapper/AnyValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		idID,
		titleID,
		typeID,
		valueID,
		enumID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"id"},
		Identity{"title"},
		Identity{"type"},
		Identity{"value"},
		Identity{"enum"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool CardSetting::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[idID], idID, element },
			{ fieldID[titleID], titleID, element },
			{ fieldID[typeID], typeID, element },
			{ fieldID[valueID], valueID, element },
			{ fieldID[enumID], enumID, element },
		},
	}.withType(&typeid(CardSetting)));
	return true;
} //CardSetting::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique CardSetting::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(CardSetting))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case idID:
			return std::make_unique<ValueWrap<String>>(m_ID);
		case titleID:
			return std::make_unique<ValueWrap<String>>(m_title);
		case typeID:
			return std::make_unique<ValueWrap<String>>(m_type);
		case valueID:
			return std::make_unique<AnyValueWrap>(*m_value);
		case enumID:
			return std::make_unique<ContainerWrap<std::vector<speckle::utility::String>>>(m_enum);
		default:
			return nullptr;	//Requested an unknown index
	}
} //CardSetting::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void CardSetting::setDefault() {
	m_ID.clear();
	m_title.clear();
	m_type.clear();
	m_value->setDefault();
	m_enum.clear();
} //CardSetting::setDefault
