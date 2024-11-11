#ifndef SPECKLE_RECORD_ELEMENT_MATERIAL_QUANTITY
#define SPECKLE_RECORD_ELEMENT_MATERIAL_QUANTITY

#include "Active/Serialise/Package/Package.h"
#include "Active/Setting/Values/Measurement/Units/LengthUnit.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::record::element::quants {
	
	/*!
	 Measured material quantities extracted from an element
	 
	 This object pairs a material (BuildingMaterial in Archicad) with measurements of surface are ands volume (pssibly more in future).
	 An element may be constructed of many materials, e.g. the skins of a wall, in which case the quants extracted from the element will
	 be an array of MaterialQuantity.
	 */
	class MaterialQuantity final : public active::serialise::Package {
	public:

		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<MaterialQuantity>;
			///Shared pointer
		using Shared = std::shared_ptr<MaterialQuantity>;
			///Optional
		using Option = std::optional<MaterialQuantity>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		MaterialQuantity() {}
		/*!
		 Constructor
		 @param materialID The ID of the measured material record
		 @param area The measured surface area (see notes on member variable)
		 @param volume The measured volume of the material
		 */
		MaterialQuantity(const database::BIMRecordID& materialID, double area = 0.0, double volume = 0.0) :
				m_materialID{materialID}, m_area{area}, m_volume{volume} {}

		// MARK: - Functions (const)
		
		/*!
		 Get the ID of the measured material
		 @return The material record ID (in the BIM attribute database)
		 */
		const database::BIMRecordID& getMaterialID() const { return m_materialID; }
		/*!
		 Get the measure surface area of the material
		 @return The material surface area
		 */
		double getArea() const { return m_area; }
		/*!
		 Get the measure volume of the material
		 @return The material volume
		 */
		double getVolume() const { return m_volume; }

		// MARK: - Functions (mutating)

		
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
			///Surface area of the material. Total surface area for basic elements, area of the primary face for 'skinned' elements, e.g. slab top
		double m_area = 0.0;
			///Volume of the material
		double m_volume = 0.0;
			///The BIM record unit of length measurement
		std::optional<active::measure::LengthType> m_unit = active::measure::LengthType::metre;
			///ID of the measured building material
		database::BIMRecordID m_materialID;
	};
	
	using MaterialQuantityList = std::vector<MaterialQuantity>;

}

#endif	//SPECKLE_RECORD_ELEMENT_MATERIAL_QUANTITY
