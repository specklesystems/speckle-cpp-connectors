#include "Speckle/Primitive/Polyline/Polyline.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Inventory/Identity.h"
#include "Speckle/Serialise/Collection/FinishProxy.h"

#include <array>

using namespace active::serialise;
using namespace speckle::primitive;
using namespace speckle::serialise;

namespace {

		///Serialisation fields
	enum FieldIndex {
		pointsID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"values"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Polyline::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[pointsID], pointsID, element },
		},
	}.withType(&typeid(Polyline)));
	return base::fillInventory(inventory);
} //Polyline::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Polyline::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Polyline))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
	case pointsID:
			return std::make_unique<ContainerWrap<std::vector<double>>>(m_points);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Polyline::getCargo


/*--------------------------------------------------------------------
	Use a manager in (de)serialisation processes
 
	management: The management to use
  --------------------------------------------------------------------*/
void Polyline::useManagement(Management* management) const {

} //Polyline::useManagement
