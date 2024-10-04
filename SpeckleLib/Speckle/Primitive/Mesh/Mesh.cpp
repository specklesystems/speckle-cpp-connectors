#include "Speckle/Primitive/Mesh/Mesh.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Inventory/Identity.h"

#include <array>

using namespace active::serialise;
using namespace speckle::primitive;

namespace {

		///Serialisation fields
	enum FieldIndex {
		vertexID,
		faceID,
		colorID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"vertices"},
		Identity{"faces"},
		Identity{"colors"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Mesh::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[vertexID], vertexID, element },
			{ fieldID[faceID], faceID, element },
			{ fieldID[colorID], colorID, element },
		},
	}.withType(&typeid(Mesh)));
	return base::fillInventory(inventory);
} //Mesh::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Mesh::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Mesh))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
	case vertexID:
			return std::make_unique<ContainerWrap<std::vector<double>>>(vertices);
		case faceID:
			return std::make_unique<ContainerWrap<std::vector<int>>>(faces);
		case colorID:
			return std::make_unique<ContainerWrap<std::vector<int>>>(colors);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Mesh::getCargo
