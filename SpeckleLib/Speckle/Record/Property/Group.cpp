#include "Speckle/Record/Property/Group.h"

#include "Speckle/Database/BIMPropertyDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Property/Setting.h"
#include "Speckle/Serialise/Types/Str256.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::property;
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
Group::Group() {
} //Group::Group


/*--------------------------------------------------------------------
	Constructor
 
	ID: The template ID
  --------------------------------------------------------------------*/
Group::Group(const database::BIMRecordID& ID) : base{ID, propertyGroupTableID} {
} //Group::Group


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
Group::Group(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID) : base{ID, tableID} {
} //Group::Group


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	source: An Archicad property group to copy
  --------------------------------------------------------------------*/
Group::Group(const API_PropertyGroup& source) : base{source.guid, propertyGroupTableID}, m_name(source.name), m_description(source.description) {
} //Group::Group
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Group::fillInventory(Inventory& inventory) const {
		//Implement when required
	return base::fillInventory(inventory);
} //Group::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Group::getCargo(const Inventory::Item& item) const {
		//Implement when required
	return base::getCargo(item);
} //Group::getCargo
