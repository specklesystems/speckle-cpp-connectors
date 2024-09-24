#ifndef SPECKLE_PRIMITIVE_MESH
#define SPECKLE_PRIMITIVE_MESH

#include "Active/Serialise/Package/Package.h"

namespace speckle::primitive {
	
	/*!
	 Class for a 3D mesh
	*/
	class Mesh : public active::serialise::Package {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Mesh() {}

		// MARK: - Functions (const)
		
		
		// MARK: - Serialisation
		
		/*!
		 Fill an inventory with the package items
		 @param inventory The inventory to receive the package items
		 @return True if the package has added items to the inventory
		*/
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure)
		*/
		active::serialise::Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		
	private:
		
	};
	
}

#endif	//SPECKLE_PRIMITIVE_MESH
