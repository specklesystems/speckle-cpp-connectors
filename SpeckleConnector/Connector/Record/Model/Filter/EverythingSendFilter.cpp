#include "Connector/Record/Model/Filter/EverythingSendFilter.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

#include <algorithm>
#include <array>

using namespace active::serialise;
using namespace connector::database;
using namespace connector::record;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		selectedElemID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"selectedObjectIds"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool EverythingSendFilter::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[selectedElemID], selectedElemID, element },
		},
	}.withType(&typeid(EverythingSendFilter)));
	return base::fillInventory(inventory);
} //EverythingSendFilter::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique EverythingSendFilter::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(EverythingSendFilter))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case selectedElemID:
			return std::make_unique<ContainerWrap<ElementIDList>>(m_emptyList);
		default:
			return nullptr;	//Requested an unknown index
	}
} //EverythingSendFilter::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void EverythingSendFilter::setDefault() {
	m_emptyList.clear();
} //EverythingSendFilter::setDefault
