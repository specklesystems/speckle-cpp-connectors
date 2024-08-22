#include "Connector/Database/Model/Card/ModelCardDatabase.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ValueSettingWrap.h"
#include "Active/Setting/ValueSetting.h"

#include <array>

using namespace active::serialise;
using namespace active::setting;
using namespace connector::database;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		model,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"model"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ModelCardDatabase::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[model], model, element },
		},
	}.withType(&typeid(ModelCardDatabase)));
	return true;
} //ModelCardDatabase::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ModelCardDatabase::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ModelCardDatabase))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case model:
			return std::make_unique<CargoHold<ValueSettingWrap, ValueSetting>>();	//NB: This is a placeholder until we define the content
		default:
			return nullptr;	//Requested an unknown index
	}
} //ModelCardDatabase::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ModelCardDatabase::setDefault() {
} //ModelCardDatabase::setDefault
