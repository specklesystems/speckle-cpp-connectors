#ifndef SPECKLE_RECORD_ELEMENT_STAIR
#define SPECKLE_RECORD_ELEMENT_STAIR

#include "Speckle/Record/Element/StairRiser.h"
#include "Speckle/Record/Element/ModelElement.h"
#include "Speckle/Record/Element/Interface/Assembly/Path.h"
#include "Speckle/Record/Element/Interface/Risers.h"

namespace speckle::record::element {
	
	class StairRiser;
	
	/*!
	 BIM stair class
	 */
	class Stair : public ModelElement, public Risers {
	public:

		// MARK: - Types
		
		using base = ModelElement;
			///Unique pointer
		using Unique = std::unique_ptr<Stair>;
			///Shared pointer
		using Shared = std::shared_ptr<Stair>;
			///Optional
		using Option = std::optional<Stair>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Stair();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The stair element ID (AC database, e.g. floor plan, 3D)
		 */
		Stair(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Stair(const Stair& source);
		/*!
		 Destructor
		 */
		~Stair();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		Stair* clonePtr() const override { return new Stair{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the BIM application parent table ID
		 @return The BIM table ID
		 */
		virtual database::BIMRecordID getTableID() const override { return ModelElement::getTableID(); }
		/*!
		 Get the element body
		 @return nullptr (Stairs don't explicitly have a 3D body - this comes from its child parts)
		 */
		virtual Body* getBody() const override { return nullptr; }
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
		/*!
		 Load the element memo structure (elements must override according to requirements)
		 @param filter Filter bits specifying memo requirements
		 */
		virtual void loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const override;
		
	private:
		class Data;
			///The stair data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_STAIR
