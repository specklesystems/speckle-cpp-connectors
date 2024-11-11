#ifndef SPECKLE_RECORD_ELEMENT_ROOF
#define SPECKLE_RECORD_ELEMENT_ROOF

#include "Speckle/Record/Element/ModelElement.h"
#include "Speckle/Record/Element/Interface/Assembly/Segment.h"

namespace speckle::record::element {
	
	/*!
	 BIM roof class
	 */
	class Roof : public ModelElement {
	public:

		// MARK: - Types
		
		using base = ModelElement;
			///Unique pointer
		using Unique = std::unique_ptr<Roof>;
			///Shared pointer
		using Shared = std::shared_ptr<Roof>;
			///Optional
		using Option = std::optional<Roof>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Roof();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		Roof(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Roof(const Roof& source);
		/*!
		 Move constructor
		 @param source The object to move
		 */
		Roof(Roof&& source) noexcept;
		/*!
		 Destructor
		 */
		~Roof();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		Roof* clonePtr() const override { return new Roof{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the composition of materials in the element
		 @return The material composition (element with ordered material composition should override)
		 */
		Composition getComposition() const override;
		/*!
		 Get the element material (applicable to elements with a single, homogenous material)
		 @return The element material (nullopt if not applicable to the element)
		 */
		std::optional<record::attribute::Material> getMaterial() const override;
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
		
	private:
		class Data;
			///The beam data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_ROOF
