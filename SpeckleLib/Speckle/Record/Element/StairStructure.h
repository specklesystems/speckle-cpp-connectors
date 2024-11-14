#ifndef SPECKLE_RECORD_ELEMENT_STAIR_STRUCTURE
#define SPECKLE_RECORD_ELEMENT_STAIR_STRUCTURE

#include "Speckle/Record/Element/ModelElement.h"

namespace speckle::record::element {
	
	class StructuredStair;
	
	/*!
	 BIM stair structure class (component in a stair assembly, e.g. stringers)
	 */
	class StairStructure : public ModelElement {
	public:

		// MARK: - Types
		
		using base = ModelElement;
			///Unique pointer
		using Unique = std::unique_ptr<StairStructure>;
			///Shared pointer
		using Shared = std::shared_ptr<StairStructure>;
			///Optional
		using Option = std::optional<StairStructure>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		StairStructure();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		StairStructure(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		StairStructure(const StairStructure& source);
		/*!
		 Move constructor
		 @param source The object to move
		 */
		StairStructure(StairStructure&& source) noexcept;
		/*!
		 Destructor
		 */
		~StairStructure();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		StairStructure* clonePtr() const override { return new StairStructure{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the element material (applicable to elements with a single, homogenous material)
		 @return The element material (nullopt if not applicable to the element)
		 */
		virtual std::optional<record::attribute::Material> getMaterial() const override;
		/*!
		 Get a spatial measurement from this element (area/volume, as used for material quantities)
		 @return The element measurement (empty if unavailable)
		 */
		SpatialMeasure getSpatialMeasure() const override;
#ifdef ARCHICAD
		/*!
		 Get the (immutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		const API_Elem_Head& getHead() const override;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
		 Get the (mutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		API_Elem_Head& getHead() override;
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
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		/*!
		 Set to the default package content
		 */
		void setDefault() override;
		
	protected:
		friend class StructuredStair;
		
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param structure The structure element data
		 @param tableID The parent table ID
		 */
		StairStructure(const API_StairStructureType& structure, const speckle::utility::Guid& tableID);
#endif
		
	private:
		class Data;
			///The stair structure data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_STAIR_STRUCTURE
