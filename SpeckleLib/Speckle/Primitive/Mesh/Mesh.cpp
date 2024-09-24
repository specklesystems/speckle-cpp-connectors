#include "Speckle/Primitive/Mesh/Mesh.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Inventory/Identity.h"

#include <array>

using namespace active::serialise;
using namespace speckle::primitive;

namespace {

		///Serialisation fields
	enum FieldIndex {
		vertexID,
		faceID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"vertices"},
		Identity{"faces"},
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
			//{ fieldID[vertexID], vertexID, vertices::size(), std::nullopt, !vertices::empty() },
			//{ fieldID[elementID], elementID, faces::size(), std::nullopt, !faces::empty() },
		},
	}.withType(&typeid(Mesh)));
	return true;
} //Mesh::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Mesh::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Mesh))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case vertexID:
			return nullptr;	//TODO: Implement vertices array
		case faceID:
			return nullptr;	//TODO: Implement faces array
		default:
			return nullptr;	//Requested an unknown index
	}
} //Mesh::getCargo
