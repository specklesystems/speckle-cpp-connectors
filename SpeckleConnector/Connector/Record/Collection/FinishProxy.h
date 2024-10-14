#ifndef CONNECTOR_RECORD_COLLECTION_MATERIAL_PROXY
#define CONNECTOR_RECORD_COLLECTION_MATERIAL_PROXY

#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Record/Attribute/Finish.h"
#include "Speckle/Utility/String.h"

namespace connector::record {
	
	/*!
	 A proxy record binding a surface finishes to meshes
	 */
	class FinishProxy : public speckle::database::BIMRecord {
	public:
				
		using base = speckle::database::BIMRecord;
		
		// MARK: - Constructors
		
		/*!
			Constructor
			@param finish The proxy surface finish
			@param meshID The list of mesh IDs the finish is applied to
		*/
		FinishProxy(const speckle::record::attribute::Finish& finish, const std::unordered_set<active::utility::Guid>& meshID) :
				base{speckle::utility::Guid{true}, speckle::utility::Guid{}, std::nullopt}, m_finish{finish} {
				std::copy(meshID.begin(), meshID.end(), std::back_inserter(m_meshID));
		}

		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required, but "Base" is still considered a type on its own)
		 */
		speckle::utility::String getSpeckleType() const override { return "Objects.Other.RenderMaterialProxy"; }
		
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
