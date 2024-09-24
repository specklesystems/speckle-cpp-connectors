#ifndef CONNECTOR_RECORD_RECORD_COLLECTiON
#define CONNECTOR_RECORD_RECORD_COLLECTiON

#include "Active/Container/Vector.h"
#include "Active/Serialise/Package/Package.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Utility/String.h"

namespace connector::record {
	
	/*!
	 Container for a collection of elements (and potentially tables of associated attributes) for Speckle commits
	 
	 The container only stores element indices - database operations (including serialisation) will lookup records from a specified  BIMDatabase on
	 demand.
	 
	 This container can used hierarchically, so an collection can be nested within another collection. The current structure is:
	 - Root
	 	- Element containers dividing elements by level/storey
	 		- Element containers dividing elements by classification
	 			- [nested classification leaf nodes)
	 	- Associated attributes, e.g. classification table (future)
	 
	 Any level in the hierarchy may contain element indices, although this is currently unlikely at the root level (all elements have a level/storey)
	 Each container should be named appropriately, e.g. a level/storey collection should be named to match the level/storey.
	 
	 Note that the serialisation is currently implemented for sending only. Receive can be added as required
	 */
	class RecordCollection : public std::vector<speckle::database::RecordID>, public active::serialise::Package {
	public:
		
		// MARK: - Types
		
		using base = std::vector<speckle::database::RecordID>;
		using Children = std::vector<RecordCollection>;
		
		// MARK: - Constructors
		
		using base::base;
				
		// MARK: - Functions (const)
		
		/*!
		 Get the container name
		 @return The container name
		 */
		const speckle::utility::String& getName() const { return m_name; }
		/*!
		 Get the child collections
		 @return The child collections nested under this collection
		 */
		const Children& getChildren() const;
				
		// MARK: - Functions (mutating)
		
		/*!
		 Set the container name
		 @param name The container name
		 */
		void setName(const speckle::utility::String& name) { m_name = name; }
		/*!
		 Add a child collection
		 @param child The child collection to add
		 */
		void addChild(RecordCollection&& child);
		
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
			///Child nodes of this collection
		Children m_children;
			///The collection name
		speckle::utility::String m_name;
	};
	
}

#endif	//CONNECTOR_RECORD_RECORD_COLLECTiON
