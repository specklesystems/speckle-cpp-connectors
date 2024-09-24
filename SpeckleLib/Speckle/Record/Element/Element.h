#ifndef SPECKLE_RECORD_ELEMENT
#define SPECKLE_RECORD_ELEMENT

#include "Speckle/Database/Content/Record.h"
#include "Speckle/Utility/String.h"

namespace speckle::primitive {
	class Mesh;
}

namespace speckle::record::element {
	
	/*!
	 Base BIM element class
	 */
	class Element : public speckle::database::Record {
	public:
		
			///An element 3D body primitive
		using Body = std::vector<primitive::Mesh>;

		// MARK: - Types
		
		using base = speckle::database::Record;
			///Unique pointer
		using Unique = std::unique_ptr<Element>;
			///Shared pointer
		using Shared = std::shared_ptr<Element>;
			///Optional
		using Option = std::optional<Element>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Element();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 */
		Element(const API_Element& elemData);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Element(const Element& source);
		/*!
		 Destructor
		 */
		~Element();

		/*!
			Object cloning
			@return A clone of this object
		*/
		Element* clonePtr() const override { return new Element{*this}; }


		// MARK: - Functions (const)

		/*!
			Get the element body
			@return An array of meshes from the element body (nullptr if no body data is available)
		*/
		virtual Body* getBody() const;
#ifdef ARCHICAD
		/*!
			Get the (immutable) API element header data
			@return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		*/
		virtual const API_Elem_Head& getHead() const;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
			Get the (mutable) API element header data
			@return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		*/
		virtual API_Elem_Head& getHead();
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

#endif	//SPECKLE_RECORD_ELEMENT
