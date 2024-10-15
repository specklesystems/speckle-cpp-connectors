#ifndef SPECKLE_RECORD_ELEMENT
#define SPECKLE_RECORD_ELEMENT

#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Record/Element/Interface/Part.h"
#include "Speckle/Record/Attribute/Storey.h"
#include "Speckle/Utility/String.h"

namespace speckle::primitive {
	class Mesh;
}

namespace speckle::record::element {
	
	class Memo;
	
	/*!
	 Base BIM element class
	 */
	class Element : public speckle::database::BIMRecord {
	public:
		
			///An element 3D body primitive
		using Body = std::vector<primitive::Mesh>;

		// MARK: - Types
		
		using base = speckle::database::BIMRecord;
			///Unique pointer
		using Unique = std::unique_ptr<Element>;
			///Shared pointer
		using Shared = std::shared_ptr<Element>;
			///Optional
		using Option = std::optional<Element>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Element();
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 @param unit The record unit type
		 */
		Element(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID,
				std::optional<active::measure::LengthType> unit = active::measure::LengthType::metre);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Element(const Element& source);
		/*!
		 Destructor
		 */
		~Element();

		// MARK: - Functions (const)

		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		virtual speckle::utility::String getSpeckleType() const override { return "Objects.BuiltElements.Element:Objects.BuiltElements.Element"; }
		/*!
		 Get the elmeent type name, e.g. "Wall", "Roof" etc
		 @return The type name
		 */
		virtual speckle::utility::String getTypeName() const;
		/*!
		 Get the element storey
		 @return The element storey (nullopt if the element isn't linked to a storey)
		 */
		virtual attribute::Storey::Option getStorey() const;
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
		virtual const API_Elem_Head& getHead() const = 0;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
		 Get the (mutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		virtual API_Elem_Head& getHead() = 0;
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
		/*!
		 Load the element memo structure (elements must override according to requirements)
		 @param filter Filter bits specifying memo requirements
		 */
		virtual void loadMemo(Part::filter_bits filter, std::unique_ptr<Memo>& memo) const;
		
	private:
		class Data;
			///The element data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT
