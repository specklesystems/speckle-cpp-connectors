#include "Speckle/Database/Content/Record.h"

#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::utility;

#include <array>

namespace {

		///Serialisation fields
	enum FieldIndex {
		speckleIDID,
		speckleTypeID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"id"},
		Identity{"speckle_type"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Record::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ Identity{fieldID[speckleIDID]}, active::database::record::FieldIndex::idIndex, element },
		},
	}.withType(&typeid(base)));
	inventory.merge(Inventory{
		{
			{ Identity{fieldID[speckleTypeID]}, speckleTypeID, element },
		},
	}.withType(&typeid(Record)));
	return true;
} //Record::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Record::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Record))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case speckleTypeID:
				//If the type is undefined, we're in a serialisation process and should populate it with whatever the object says it is
			if (!m_type)
				m_type = getSpeckleType();
			return std::make_unique<StringWrap>(*m_type);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Record::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Record::setDefault()  {
		//Ensure the content starts with an empty (defined) string for deserialisation so we can discover the incoming type
	m_type = String{};
} //Record::setDefault
