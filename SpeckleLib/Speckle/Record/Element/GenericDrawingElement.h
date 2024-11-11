#ifndef SPECKLE_RECORD_MODEL_ELEMENT
#define SPECKLE_RECORD_MODEL_ELEMENT

#include "Speckle/Record/Element/Element.h"

namespace speckle::record::element {
	
	/*!
	 Base class for generic drawing (2D) elements, i.e. not defined by a specific element type
	 */
	class GenericDrawingElement : public Element {
	public:

		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<GenericDrawingElement>;
			///Shared pointer
		using Shared = std::shared_ptr<GenericDrawingElement>;
			///Optional
		using Option = std::optional<GenericDrawingElement>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		GenericDrawingElement();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		GenericDrawingElement(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		GenericDrawingElement(const GenericDrawingElement& source);
		/*!
		 Destructor
		 */
		~GenericDrawingElement();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GenericDrawingElement* clonePtr() const override { return new GenericDrawingElement{*this}; }


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

#endif	//SPECKLE_RECORD_MODEL_ELEMENT
