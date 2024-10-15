#ifndef SPECKLE_RECORD_GENERIC_ELEMENT
#define SPECKLE_RECORD_GENERIC_ELEMENT

#include "Speckle/Record/Element/Element.h"

namespace speckle::record::element {
	
	/*!
	 Catch-all class for elements that are not represented by a specific class
	 */
	class GenericElement : public Element {
	public:
		
			///An element 3D body primitive
		using Body = std::vector<primitive::Mesh>;

		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<GenericElement>;
			///Shared pointer
		using Shared = std::shared_ptr<GenericElement>;
			///Optional
		using Option = std::optional<GenericElement>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		GenericElement();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		GenericElement(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		GenericElement(const GenericElement& source);
		/*!
		 Destructor
		 */
		~GenericElement();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GenericElement* clonePtr() const override { return new GenericElement{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		virtual speckle::utility::String getSpeckleType() const override { return "Objects.BuiltElements.GenericElement:Objects.BuiltElements.GenericElement"; }
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

#endif	//SPECKLE_RECORD_GENERIC_ELEMENT
