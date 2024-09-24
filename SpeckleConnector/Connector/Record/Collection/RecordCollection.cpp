#include "Connector/Record/Collection/RecordCollection.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::utility;

#include <array>

namespace {

		///Serialisation fields
	enum FieldIndex {
		nameID,
		elementID,
		childrenID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
		Identity{"element"},
		Identity{"child"},
	};

}


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool RecordCollection::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
			{ fieldID[elementID], elementID, base::size(), std::nullopt, !base::empty() },
			{ fieldID[childrenID], childrenID, m_children.size(), std::nullopt, !m_children.empty() },
		},
	}.withType(&typeid(RecordCollection)));
	return true;
} //RecordCollection::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique RecordCollection::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(RecordCollection))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			return std::make_unique<StringWrap>(m_name);
		case elementID:
			return nullptr;	//TODO: Implement - need to interrogate BIM database for element and return as cargo
		case childrenID:
			if (item.available < m_children.size())
				return std::make_unique<PackageWrap>(m_children[item.available]);
			return nullptr;
		default:
			return nullptr;	//Requested an unknown index
	}
} //RecordCollection::getCargo
