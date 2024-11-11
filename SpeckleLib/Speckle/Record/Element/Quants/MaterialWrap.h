#ifndef SPECKLE_RECORD_ELEMENT_MATERIAL_WRAP
#define SPECKLE_RECORD_ELEMENT_MATERIAL_WRAP

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::record::element::quants {
	
	/*!
	 Wrapper for a material exported with a quantity
	 
	 NB: This wrapper primarily exports the material name. In future this might be linked to a material proxy, in which case this simple wrapper
	 will become a MaterialProxy
	 */
	class MaterialWrap final : public active::serialise::Package {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		MaterialWrap(const database::BIMRecordID& materialID);
		
		//MARK: - Serialisation

		/*!
			Fill an inventory with the cargo items
			@param inventory The inventory to receive the cargo items
			@return True if items have been added to the inventory
		*/
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
			Get the specified cargo
			@param item The inventory item to retrieve
			@return The requested cargo (nullptr on failure)
		*/
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		
	private:
			///The wrapped material name
		utility::String m_name;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_MATERIAL_WRAP
