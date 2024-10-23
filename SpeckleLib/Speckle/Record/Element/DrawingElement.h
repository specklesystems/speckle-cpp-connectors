#ifndef SPECKLE_RECORD_DISPLAY_ELEMENT
#define SPECKLE_RECORD_DISPLAY_ELEMENT

#include "Speckle/Record/Element/Element.h"

namespace speckle::record::element {
	
	/*!
	 Base class for drawing (illustrative) elements, i.e. appearing in 2D only and typically used in 2D drawings
	 */
	class DrawingElement : public Element {
	public:
		
		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<DrawingElement>;
			///Shared pointer
		using Shared = std::shared_ptr<DrawingElement>;
			///Optional
		using Option = std::optional<DrawingElement>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		DrawingElement();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		DrawingElement(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		DrawingElement(const DrawingElement& source);
		/*!
		 Destructor
		 */
		~DrawingElement();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		DrawingElement* clonePtr() const override { return new DrawingElement{*this}; }


		// MARK: - Functions (const)

#ifdef ARCHICAD
		/*!
		 Get the (immutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		virtual const API_Elem_Head& getHead() const override;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
		 Get the (mutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		virtual API_Elem_Head& getHead() override;
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
			///The element data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_DISPLAY_ELEMENT
