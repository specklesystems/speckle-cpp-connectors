#ifndef SPECKLE_RECORD_PROPERTY_GROUP
#define SPECKLE_RECORD_PROPERTY_GROUP

#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Record/Property/Value.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::property {
	
	class Setting;
	
	/*!
	 Class defining the characteristics of a property group
	 
	 Properties are typically associated with a group
	 Property groups are persisted in the BIM property database
	 */
	class Group : public speckle::database::BIMRecord {
	public:

		// MARK: - Types
		
		using base = speckle::database::BIMRecord;
			///Unique pointer
		using Unique = std::unique_ptr<Group>;
			///Shared pointer
		using Shared = std::shared_ptr<Group>;
			///Optional
		using Option = std::optional<Group>;
		
		// MARK: - Constants
		
			///Identifier for a property group table
		inline static utility::Guid propertyGroupTableID{utility::String{"cbe185dc-5011-4325-9651-1852056a04de"}};

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Group();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param source An Archicad property group to copy
		 */
		Group(const API_PropertyGroup& source);
#endif
		/*!
		 Constructor
		 @param ID The group ID
		 */
		Group(const database::BIMRecordID& ID);
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 */
		Group(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID);

		// MARK: - Functions (const)

		/*!
		 Get the group name
		 @return The group name
		 */
		speckle::utility::String getName() const { return m_name; }
		
		// MARK: - Functions (mutating)

			//TODO: Add methods as required
		
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

	private:
			///Name
		speckle::utility::String m_name;
			///Description
		speckle::utility::String m_description;
	};

}

#endif	//SPECKLE_RECORD_PROPERTY_GROUP
