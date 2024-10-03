#ifndef SPECKLE_DATABASE_BIM_RECORD
#define SPECKLE_DATABASE_BIM_RECORD

#include "Active/Database/Content/Record.h"
#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::database {

	/*!
	 Base class for a database record
	 */
	class BIMRecord : public active::database::Record<BIMRecordID>{
	public:

		// MARK: - Types
		
		using base = active::database::Record<BIMRecordID>;
			///Unique pointer
		using Unique = std::unique_ptr<BIMRecord>;
			///Shared pointer
		using Shared = std::shared_ptr<BIMRecord>;
			///Optional
		using Option = std::optional<BIMRecord>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		BIMRecord() : base{active::utility::Guid{true}, active::utility::Guid{true}} {}	//TODO: Implement a better default for the ID
		/*!
		 Constructor
		 @param ID The record ID
		 */
		BIMRecord(speckle::utility::Guid ID, speckle::utility::Guid::Option globID = std::nullopt) :	//TODO: Implement a better default for the ID
				base{ID, globID.value_or(active::utility::Guid{true})} {}
		/*!
		 Destructor
		 */
		virtual ~BIMRecord() {}
		
		// MARK: - Functions (const)

		
		// MARK: - Functions (mutating)

		
		// MARK: - Serialisation
		
		/*!
			Fill an inventory with the package items
			@param inventory The inventory to receive the package items
			@return True if the package has added items to the inventory
		*/
		bool fillInventory(active::serialise::Inventory& inventory) const override;
	};
	
}

#endif	//SPECKLE_DATABASE_BIM_RECORD
