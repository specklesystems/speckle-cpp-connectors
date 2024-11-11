#include "Speckle/Record/Element/Quants/MaterialWrap.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Record/Attribute/Material.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::record::element::quants;
using namespace speckle::utility;

namespace {

	///Serialisation fields
	enum FieldIndex {
		nameID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
	};
	
}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
MaterialWrap::MaterialWrap(const BIMRecordID& materialID) : m_name{Material{materialID}.getName()} {
} //MaterialWrap::MaterialWrap


/*--------------------------------------------------------------------
	Fill an inventory with the cargo items
 
	inventory: The inventory to receive the cargo items
 
	return: True if items have been added to the inventory
  --------------------------------------------------------------------*/
bool MaterialWrap::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
		},
	}.withType(&typeid(MaterialWrap)));
	return true;
} //MaterialWrap::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique MaterialWrap::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(MaterialWrap))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			return std::make_unique<ValueWrap<String>>(m_name);
		default:
			return nullptr;	//Requested an unknown index
	}
} //MaterialWrap::getCargo
