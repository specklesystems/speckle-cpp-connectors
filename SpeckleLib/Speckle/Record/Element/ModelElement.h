#ifndef SPECKLE_RECORD_MODEL_ELEMENT
#define SPECKLE_RECORD_MODEL_ELEMENT

#include "Speckle/Record/Element/Element.h"

namespace speckle::record::element {
	
	/*!
	 Base class for model elements, i.e. with 3D bodies and typically representing the project model
	 */
	class ModelElement : public Element {
	public:

		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<ModelElement>;
			///Shared pointer
		using Shared = std::shared_ptr<ModelElement>;
			///Optional
		using Option = std::optional<ModelElement>;
			///A model element 3D body primitive
		using Body = std::vector<primitive::Mesh>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		ModelElement();
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 @param unit The record unit type
		 */
		ModelElement(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID,
					 std::optional<active::measure::LengthType> unit = active::measure::LengthType::metre);
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		ModelElement(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ModelElement(const ModelElement& source);
		/*!
		 Destructor
		 */
		~ModelElement();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		ModelElement* clonePtr() const override { return new ModelElement{*this}; }


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
