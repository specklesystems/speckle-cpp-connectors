#ifndef CONNECTOR_RECORD_COLLECTION_MATERIAL_PROXY
#define CONNECTOR_RECORD_COLLECTION_MATERIAL_PROXY

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Record/Attribute/Finish.h"
#include "Speckle/Utility/String.h"

namespace connector::record {
	
	/*!
	 A proxy record binding a surface finishes to meshes
	 */
	class FinishProxy : public active::serialise::Package {
	public:
				
		// MARK: - Constructors
		
		/*!
			Constructor
			@param finish The proxy surface finish
			@param meshID The list of mesh IDs the finish is applied to
		*/
		FinishProxy(const speckle::record::attribute::Finish& finish, const std::unordered_set<active::utility::Guid>& meshID) :
				m_finish{finish} { std::copy(meshID.begin(), meshID.end(), std::back_inserter(m_meshID)); }

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
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		
	private:
			///The proxy surface finish
		speckle::record::attribute::Finish m_finish;
			///The list of mesh IDs the finish is applied to
		std::vector<active::utility::Guid> m_meshID;
	};

}

#endif	//CONNECTOR_RECORD_COLLECTION_MATERIAL_PROXY
