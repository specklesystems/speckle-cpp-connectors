#ifndef SPECKLE_DATABASE_RECORD
#define SPECKLE_DATABASE_RECORD

#include "Active/Database/Content/Record.h"
#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Database/Identity/RecordID.h"

namespace speckle::database {

	/*!
	 Base class for a database record
	 */
	class Record : public active::database::Record<RecordID> {
	public:

		// MARK: - Types
		
		using base = active::database::Record<RecordID>;
			///Unique pointer
		using Unique = std::unique_ptr<Record>;
			///Shared pointer
		using Shared = std::shared_ptr<Record>;
			///Optional
		using Option = std::optional<Record>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Record() : base{active::utility::Guid{true}.operator active::utility::String(),
				active::utility::Guid{true}.operator active::utility::String()} {}	//TODO: Implement a better default for the ID
		/*!
		 Constructor
		 @param ID The record ID
		 */
		Record(speckle::utility::String ID, speckle::utility::String::Option globID = std::nullopt) :
				base{ID, globID.value_or(active::utility::Guid{true}.operator active::utility::String())} {}
		/*!
		 Destructor
		 */
		virtual ~Record() {}
		
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

#endif	//SPECKLE_DATABASE_RECORD
