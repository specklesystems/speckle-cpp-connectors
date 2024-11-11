#ifndef SPECKLE_RECORD_ATTRIBUTE_STOREY
#define SPECKLE_RECORD_ATTRIBUTE_STOREY

#include "Speckle/Record/Attribute/Attribute.h"

namespace speckle::record::attribute {
	
	/*!
	 A storey or level in a building
	 
	 Represented in Archicad by `API_StoryType`
	 */
	class Storey : public Attribute {
	public:

		// MARK: - Types
		
		using base = Attribute;
			///Unique pointer
		using Unique = std::unique_ptr<Storey>;
			///Shared pointer
		using Shared = std::shared_ptr<Storey>;
			///Optional
		using Option = std::optional<Storey>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Storey();
		/*!
		 Constructor
		 @param ID The attribute ID
		 */
		Storey(const database::BIMRecordID& ID);
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param index An index into the Archicad storey array
		 */
		Storey(short index);
		/*!
		 Constructor
		 @param attrData Archicad attribute data
		 */
		Storey(const API_StoryType& attrData);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Storey(const Storey& source);
		/*!
		 Destructor
		 */
		~Storey();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		Storey* clonePtr() const override { return new Storey{*this}; }


		// MARK: - Functions (const)

#ifdef ARCHICAD
		/*!
		 Get the (immutable) API attribute header data
		 @return The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		const API_Attr_Head& getHead() const override;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
		 Get the (mutable) API attribute header data
		 @return The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		API_Attr_Head& getHead() override;
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
		/*!
		 Confirm the internal data, either loading from the BIM application or setting a default
		 */
		void confirmData() const;
#ifdef ARCHICAD
		/*!
		 Get the storey data from the host BIM application
		 @return The storey data (for internal use to populate derived classes)
		 */
		API_StoryType getStoreyData() const;
		
			///An index into the Archicad storey array - used temporarily for looking up the storey data on demand
		mutable std::optional<short> m_storeyIndex;
#endif
		
		class Data;
			///The attribute data - mutable to support lazy loading
		mutable std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ATTRIBUTE_STOREY
