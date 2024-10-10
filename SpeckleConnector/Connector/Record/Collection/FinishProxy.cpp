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
			{ fieldID[linkedMeshID], linkedMeshID, m_meshID.size(), std::nullopt },
		},
	}.withType(&typeid(FinishProxy)));
	return true;
} //FinishProxy::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique FinishProxy::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(FinishProxy))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case materialID:
			return std::make_unique<PackageWrap>(m_finish);
		case linkedMeshID:
			return std::make_unique<ContainerWrap<std::vector<active::utility::Guid>>>(m_meshID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //FinishProxy::getCargo
