#ifndef SPECKLE_RECORD_ATTRIBUTE_FINISH
#define SPECKLE_RECORD_ATTRIBUTE_FINISH

#include "Speckle/Record/Attribute/Attribute.h"

namespace speckle::record::attribute {
	
	/*!
	 Class to represent the rendered finish on a 3D body, i.e. the surface colour/texture etc.
	 
	 In Archicad this attribute is represented by `API_MaterialType`
	 */
	class Finish : public Attribute {
	public:

		// MARK: - Types
		
		using base = Attribute;
			///Unique pointer
		using Unique = std::unique_ptr<Finish>;
			///Shared pointer
		using Shared = std::shared_ptr<Finish>;
			///Optional
		using Option = std::optional<Finish>;
		
		// MARK: - Constants
		
#ifdef ARCHICAD
			///The finishes table identifier
		static constexpr active::utility::Guid table{active::utility::Guid::fromInt(API_MaterialID)};
#endif

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Finish();
		/*!
		 Constructor
		 @param ID The attribute ID
		 */
		Finish(const database::BIMRecordID& ID);
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param attrData Archicad attribute data
		 @param tableID The ID of the parent table
		 */
		Finish(const API_Attribute& attrData, const database::BIMRecordID& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Finish(const Finish& source);
		/*!
		 Destructor
		 */
		~Finish();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		Finish* clonePtr() const override { return new Finish{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		speckle::utility::String getSpeckleType() const override { return "Objects.Other.RenderMaterial"; }
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
		
		class Data;
			///The attribute data - mutable to support lazy loading
		mutable std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ATTRIBUTE_FINISH
