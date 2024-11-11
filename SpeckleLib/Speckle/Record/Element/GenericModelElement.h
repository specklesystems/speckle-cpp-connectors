#ifndef SPECKLE_RECORD_GENERIC_MODEL_ELEMENT
#define SPECKLE_RECORD_GENERIC_MODEL_ELEMENT

#include "Speckle/Record/Element/ModelElement.h"

namespace speckle::record::element {
	
	/*!
	 Base class for generic model elements, i.e. not defined by a specific element type
	 */
	class GenericModelElement : public ModelElement {
	public:

		// MARK: - Types
		
		using base = ModelElement;
			///Unique pointer
		using Unique = std::unique_ptr<GenericModelElement>;
			///Shared pointer
		using Shared = std::shared_ptr<GenericModelElement>;
			///Optional
		using Option = std::optional<GenericModelElement>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		GenericModelElement();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		GenericModelElement(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		GenericModelElement(const GenericModelElement& source);
		/*!
		 Destructor
		 */
		~GenericModelElement();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GenericModelElement* clonePtr() const override { return new GenericModelElement{*this}; }


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

#endif	//SPECKLE_RECORD_GENERIC_MODEL_ELEMENT
