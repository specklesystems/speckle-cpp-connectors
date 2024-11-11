#include "Speckle/Record/Element/Quants/MaterialQuantity.h"

#include "Active/Serialise/Item/Wrapper/ValueOptionWrap.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Record/Attribute/Material.h"
#include "Speckle/Record/Element/Quants/MaterialWrap.h"
#include "Speckle/Serialise/Types/Units/LengthUnit.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::record::element::quants;

namespace {

	///Serialisation fields
	enum FieldIndex {
		areaID,
		volumeID,
		unitID,
		matID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"area"},
		Identity{"volume"},
		Identity{"units"},
		Identity{"material"},
	};
	
}

/*--------------------------------------------------------------------
	Fill an inventory with the cargo items
 
	inventory: The inventory to receive the cargo items
 
	return: True if items have been added to the inventory
  --------------------------------------------------------------------*/
bool MaterialQuantity::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[areaID], areaID, element },
			{ fieldID[volumeID], volumeID, element },
			{ fieldID[unitID], unitID, element },
			{ fieldID[matID], matID, element },
		},
	}.withType(&typeid(MaterialQuantity)));
	return true;
} //MaterialQuantity::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique MaterialQuantity::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(MaterialQuantity))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case areaID:
			return std::make_unique<ValueWrap<double>>(m_area);
		case volumeID:
			return std::make_unique<ValueWrap<double>>(m_volume);
		case unitID:
			return std::make_unique<ValueOptionWrap<active::measure::LengthType>>(m_unit);
		case matID:
			return std::make_unique<MaterialWrap>(m_materialID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //MaterialQuantity::getCargo
