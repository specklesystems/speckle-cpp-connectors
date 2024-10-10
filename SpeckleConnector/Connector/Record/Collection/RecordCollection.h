#ifndef CONNECTOR_RECORD_RECORD_COLLECTiON
#define CONNECTOR_RECORD_RECORD_COLLECTiON

#include "Speckle/Database/Content/Record.h"
#include "Speckle/Database/Identity/BIMIndex.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Utility/String.h"

#include <unordered_set>
#include <unordered_map>

namespace connector::record {
	
	class ProjectCollection;
	
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
	class RecordCollection : public speckle::database::Record {
	public:

		// MARK: - Types

		using base = speckle::database::Record;

		// MARK: - Constructors
		
		/*!
		 Destructor
		 */
		~RecordCollection() {}
				
		// MARK: - Functions (const)
		
		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required, but "Base" is still considered a type on its own)
		 */
		speckle::utility::String getSpeckleType() const override { return "Speckle.Core.Models.Collections.Collection"; }
		/*!
		 Get the container name
		 @return The container name
		 */
		const speckle::utility::String& getName() const { return m_name; }
		/*!
		 Find a child by name
		 @param name The required child name
		 @return A pointer to the requested child (nullptr if not found)
		 */
		RecordCollection* findChild(const speckle::utility::String& name) const;
		
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
		friend ProjectCollection;

		// MARK: - Types
		
		using Indices = std::unordered_set<speckle::database::BIMIndex>;
		using Children = std::unordered_map<speckle::utility::String, RecordCollection>;

		/*!
		 Constructor
		 @param name The collection name
		 @param project The source project
		 */
		RecordCollection(const speckle::utility::String& name, speckle::environment::Project::Shared project);
		
		/*!
		 Get a child collection by name (adding if missing)
		 @param name The child name
		 @return A pointer to the requested child (nullptr on failure, caller does not take ownership)
		 */
		RecordCollection* getChild(const speckle::utility::String& name);
		/*!
		 Add an index to the collection
		 @param index The index to add
		 @return True if the index was added (false typically means the index already exists)
		 */
		bool addIndex(const speckle::database::BIMIndex& index);
		
			///The source project for the collection
		speckle::environment::Project::Shared m_project;
			///The collection name
		speckle::utility::String m_name;
			///Child nodes of this collection
		Children m_children;
			///Indices of records in this collection
		Indices m_indices;
	};
	
}

#endif	//CONNECTOR_RECORD_RECORD_COLLECTiON
