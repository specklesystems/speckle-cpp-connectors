#include "Speckle/Database/Content/BIMRecord.h"

#include "Active/Serialise/Item/Wrapper/ValueOptionWrap.h"
#include "Speckle/Serialise/Types/Units/LengthUnit.h"
#include "Speckle/Utility/Guid.h"

#include <array>

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::utility;

using enum active::measure::LengthType;

namespace {

		///Serialisation fields
	enum FieldIndex {
		applicID,
		unitID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"applicationId"},
		Identity{"unit"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool BIMRecord::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ Identity{fieldID[applicID]}, applicID, element, !m_applicationID.empty() },
			{ Identity{fieldID[unitID]}, unitID, element, m_unit.operator bool() },
		},
	}.withType(&typeid(BIMRecord)));
	return base::fillInventory(inventory);
} //BIMRecord::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique BIMRecord::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(BIMRecord))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case applicID:
			return std::make_unique<ValueWrap<BIMRecordID>>(m_applicationID);
		case unitID:
			return std::make_unique<ValueOptionWrap<active::measure::LengthType>>(m_unit);
		default:
			return nullptr;	//Requested an unknown index
	}
} //BIMRecord::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void BIMRecord::setDefault() {
	m_applicationID.clear();
	m_unit.reset();
} //BIMRecord::setDefault
