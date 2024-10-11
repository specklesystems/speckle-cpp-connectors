#ifndef CONNECTOR_RECORD_ROOT_COLLECTiON
#define CONNECTOR_RECORD_ROOT_COLLECTiON

#include "Connector/Record/Collection/RecordCollection.h"
#include "Speckle/Serialise/Collection/FinishCollector.h"

#include <stack>

namespace speckle::record::element {
	class Element;
}

namespace connector::record {
	
	/*!
	 Root collection for sending a project model to a Speckle server
	 
	 Additional information is anticipated at the root level that will not apply at any other level in the container hierarchy, e.g.:
	 - Classification hierarchy
	 - Layers
	 - Other attributes, e.g. materials
	 Add all this supplementary data to the root container as required
	 */
	class ProjectCollection : public RecordCollection, public speckle::serialise::FinishCollector {
	public:
		
		// MARK: - Types
		
		using base = RecordCollection;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param project The source project
		 */
		ProjectCollection(speckle::environment::Project::Shared project);
		ProjectCollection(const ProjectCollection&) = delete;
		/*!
		 Destructor
		 */
		~ProjectCollection();
		
		using base::base;
				
		// MARK: - Functions (const)
				
		// MARK: - Functions (mutating)

		/*!
		 Add an element to the collection hierarchy
		 @param index The index of the element to add
		 @return True if the element was added (false typically means the element already exists)
		 */
		bool addElement(const speckle::database::BIMIndex& index);
		/*!
		 Add an element to the collection hierarchy
		 @param element The element to add
		 @return True if the element was added (false typically means the element already exists)
		 */
		bool addElement(const speckle::record::element::Element& element);
		/*!
		 Add a material proxy record to the collection
		 @param materialIndex The index of the material to add
		 @param objectID The object the material is applied to
		 @return True if the material proxy was added (false typically means the record already exists)
		 */
		bool addMaterialProxy(const speckle::database::BIMIndex& materialIndex, const speckle::database::BIMRecordID& objectID) override;
#ifdef ARCHICAD
		/*!
		 Add a ModelerAPI material to the collection (NB: These are not persistent so need to be captured by this method)
		 @param material A material
		 @param objectID The object the material is applied to
		 @return True if the material proxy was added (false typically means the record already exists)
		 */
		bool addMaterialProxy(const ModelerAPI::Material& material, const speckle::database::BIMRecordID& objectID) override;
#endif
		
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
		using FinishProxies = std::unordered_map<speckle::database::BIMIndex, std::unordered_set<active::utility::Guid>>;
		
			///Finish proxies accumulated from meshes generated from the collection elements
		FinishProxies m_finishProxies;
#ifdef ARCHICAD
		class FinishCache;
			///Finishes cached from ModelerAPI materials
		std::unique_ptr<FinishCache> m_finishes;
#endif
	};
	
}

#endif	//CONNECTOR_RECORD_ROOT_COLLECTiON
