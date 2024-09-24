#ifndef CONNECTOR_RECORD_ROOT_COLLECTiON
#define CONNECTOR_RECORD_ROOT_COLLECTiON

#include "Connector/Record/Collection/RecordCollection.h"

namespace connector::record {
	
	/*!
	 Root container for sending model data to a Speckle server
	 
	 Additional information is anticipated at the root level that will not apply at any other level in the container hierarchy, e.g.:
	 - Classification hierarchy
	 - Layers
	 - Other attributes, e.g. materials
	 Add all this supplementary data to the root container as required
	 */
	class RootCollection : public RecordCollection {
	public:
		
		// MARK: - Types
		
		using base = RecordCollection;
		
		// MARK: - Constructors
		
		using base::base;
				
		// MARK: - Functions (const)
		
				
		// MARK: - Functions (mutating)
		
		
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
	};
	
}

#endif	//CONNECTOR_RECORD_ROOT_COLLECTiON
