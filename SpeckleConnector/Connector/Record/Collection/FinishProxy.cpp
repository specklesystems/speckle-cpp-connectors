#include "Connector/Record/Collection/FinishProxy.h"

#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		materialID,
		linkedMeshID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"value"},
		Identity{"objects"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool FinishProxy::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[materialID], materialID, element },
			{ fieldID[linkedMeshID], linkedMeshID, element },
		},
	}.withType(&typeid(FinishProxy)));
	return base::fillInventory(inventory);
} //FinishProxy::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique FinishProxy::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(FinishProxy))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case materialID:
			return std::make_unique<PackageWrap>(m_finish);
		case linkedMeshID: {
			auto result = new ContainerWrap(m_meshID);
			return Cargo::Unique{result};
		}
		default:
			return nullptr;	//Requested an unknown index
	}
} //FinishProxy::getCargo
