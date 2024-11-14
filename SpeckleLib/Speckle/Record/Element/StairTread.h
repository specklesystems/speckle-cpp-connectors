#ifndef SPECKLE_RECORD_ELEMENT_STAIR_TREAD
#define SPECKLE_RECORD_ELEMENT_STAIR_TREAD

#include "Speckle/Record/Element/ModelElement.h"

namespace speckle::record::element {
	
	class Treads;
	
	/*!
	 BIM stair tread class (component in a stair assembly)
	 */
	class StairTread : public ModelElement {
	public:

		// MARK: - Types
		
		using base = ModelElement;
			///Unique pointer
		using Unique = std::unique_ptr<StairTread>;
			///Shared pointer
		using Shared = std::shared_ptr<StairTread>;
			///Optional
		using Option = std::optional<StairTread>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		StairTread();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		StairTread(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		StairTread(const StairTread& source);
		/*!
		 Move constructor
		 @param source The object to move
		 */
		StairTread(StairTread&& source) noexcept;
		/*!
		 Destructor
		 */
		~StairTread();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		StairTread* clonePtr() const override { return new StairTread{*this}; }


		// MARK: - Functions (const)

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
		friend class Treads;
		
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param tread The tread element data
		 @param tableID The parent table ID
		 */
		StairTread(const API_StairTreadType& tread, const speckle::utility::Guid& tableID);
#endif
		
	private:
		class Data;
			///The stair tread data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_STAIR_TREAD
