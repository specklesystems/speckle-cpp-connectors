#include "Speckle/Record/Attribute/Attribute.h"

#include "Speckle/Database/BIMAttributeDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::utility;

#include <array>
#include <memory>

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
	Default constructor
  --------------------------------------------------------------------*/
Attribute::Attribute() {
} //Attribute::Attribute


/*--------------------------------------------------------------------
	Get the attribute name
 
	return: The attribute name
  --------------------------------------------------------------------*/
speckle::utility::String Attribute::getName() const {
#ifdef ARCHICAD
	return getHead().name;
#endif
} //Attribute::getName


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Attribute::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
		},
	}.withType(&typeid(Attribute)));
	return base::fillInventory(inventory);
} //Attribute::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Attribute::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Attribute))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			//return std::make_unique<active::serialise::ContainerWrap>(*body);
			return std::make_unique<StringWrap>(getHead().name);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Attribute::getCargo


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the attribute data from the host BIM application
 
	return: The attribute data (for internal use to populate derived classes)
  --------------------------------------------------------------------*/
API_Attribute Attribute::getData() const {
	if (auto project = addon()->getActiveProject().lock(); project) {
		if (auto attr = project->getAttributeDatabase()->getAPIData(getBIMLink()); attr)
			return *attr;
	}
	API_Attribute attr;
	active::utility::Memory::erase(attr);
	return attr;
} //Attribute::getData
#endif
