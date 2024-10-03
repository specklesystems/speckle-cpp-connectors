#ifndef SPECKLE_DATABASE_BIM_RECORD
#define SPECKLE_DATABASE_BIM_RECORD

#include "Speckle/Database/Content/Record.h"
#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::database {

	/*!
	 Base class for a database record
	 */
	class BIMRecord : public Record {
	public:

		// MARK: - Types
		
		using base = Record;
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
		BIMRecord() : base{} {}
		/*!
		 Constructor
		 @param ID The record ID
		 */
		BIMRecord(speckle::utility::Guid ID) : base{}, m_applicationID{ID} {}
		/*!
		 Destructor
		 */
		virtual ~BIMRecord() {}
		
		// MARK: - Functions (const)

		/*!
		 Get the BIM application ID
		 @return The BIM application ID
		 */
		BIMRecordID getBIMID() const { return m_applicationID; }
		
		// MARK: - Functions (mutating)

		/*!
		 Set the BIM application ID
		 @param ID The BIM application ID
		 */
		void setBIMID(const BIMRecordID& ID) { m_applicationID = ID; }
		
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
		active::serialise::Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		
	private:
			///The BIM application record ID
		BIMRecordID m_applicationID;
	};
	
}

#endif	//SPECKLE_DATABASE_BIM_RECORD
