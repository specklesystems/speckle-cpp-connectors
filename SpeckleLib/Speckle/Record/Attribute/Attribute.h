#ifndef SPECKLE_RECORD_ATTRIBUTE
#define SPECKLE_RECORD_ATTRIBUTE

#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::attribute {
	
	/*!
	 Base BIM attribute class
	 */
	class Attribute : public speckle::database::BIMRecord {
	public:

		// MARK: - Types
		
		using base = speckle::database::BIMRecord;
			///Unique pointer
		using Unique = std::unique_ptr<Attribute>;
			///Shared pointer
		using Shared = std::shared_ptr<Attribute>;
			///Optional
		using Option = std::optional<Attribute>;

		// MARK: - Constants
		
#ifdef ARCHICAD
			///Archicad type identifier for a storey attribute table
		static constexpr int32_t storeyTableID = 0x200;
		/*!
		 Get an attribute type ID from a table ID
		 @param tableID The table ID
		 @return The attribute type ID (NB: not strictly API_AttrTypeID - can be storeyTypeID)
		 */
		static API_AttrTypeID getTypeID(const active::utility::Guid& tableID) {
			return static_cast<API_AttrTypeID>(active::utility::Guid::toInt(tableID));
		}
		/*!
		 Get an Archicad attribute index from a record ID
		 @param recordID The record ID
		 @return An attribute index
		 */
		static API_AttributeIndex getIndex(const active::utility::Guid& recordID) {
			return ACAPI_CreateAttributeIndex(static_cast<int32_t>(active::utility::Guid::toInt(recordID)));
		}
#endif

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Attribute();
		/*!
		 Constructor
		 @param ID The attribute ID
		 @param tableID The attribute table ID (attribute type)
		 */
		Attribute(const database::BIMRecordID& ID, const speckle::utility::Guid& tableID) : base{ID, tableID} {}

		// MARK: - Functions (const)

		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		speckle::utility::String getSpeckleType() const override { return "speckle::record::attribute::Attribute"; }
		/*!
		 Get the attribute name
		 @return The attribute name
		 */
		speckle::utility::String getName() const;
#ifdef ARCHICAD
		/*!
		 Get the (immutable) API attribute header data
		 @return The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		virtual const API_Attr_Head& getHead() const = 0;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
		 Get the (mutable) API attribute header data
		 @return The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		virtual API_Attr_Head& getHead() = 0;
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

	protected:
#ifdef ARCHICAD
		/*!
		 Get the attribute data from the host BIM application
		 @return The attribute data (for internal use to populate derived classes)
		 */
		API_Attribute getData() const;
#endif
	};

}

#endif	//SPECKLE_RECORD_ATTRIBUTE
