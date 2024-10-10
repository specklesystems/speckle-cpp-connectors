#ifndef SPECKLE_DATABASE_BIM_RECORD
#define SPECKLE_DATABASE_BIM_RECORD

#include "Active/Setting/Values/Measurement/Units/LengthUnit.h"
#include "Speckle/Database/Content/Record.h"
#include "Speckle/Database/Identity/BIMLink.h"
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
		 @param unit The recordc unit type
		 */
		BIMRecord(active::measure::LengthType unit = active::measure::LengthType::metre) : base{}, m_unit{unit} {}
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 @param unit The record unit type
		 */
		BIMRecord(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID,
				  active::measure::LengthType unit = active::measure::LengthType::metre) :
				base{}, m_applicationID{ID}, m_applicationTableID{tableID}, m_unit{unit} {}
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
		/*!
		 Get the BIM application parent table ID
		 @return The BIM table ID
		 */
		BIMRecordID getTableID() const { return m_applicationTableID; }
		/*!
		 Get a link to the BIM record
		 @return The BIM record link
		 */
		BIMLink getBIMLink() const { return BIMLink{ BIMLink::base{m_applicationID, m_applicationTableID} }; }
		
		// MARK: - Functions (mutating)

		/*!
		 Set the BIM application ID
		 @param ID The BIM application ID
		 */
		void setBIMID(const BIMRecordID& ID) { m_applicationID = ID; }
		/*!
		 Set the BIM application parent table ID
		 @param tableID The BIM table ID
		 */
		void setTableID(const BIMRecordID& tableID) { m_applicationTableID = tableID; }
		
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
		/*!
		 Set to the default package content
		 */
		void setDefault() override;
		
	private:
			///The BIM application record ID
		BIMRecordID m_applicationID;
			///The BIM application parent table ID
		BIMRecordID m_applicationTableID;
			///The BIM record unit of length measurement
		std::optional<active::measure::LengthType> m_unit = active::measure::LengthType::metre;
	};
	
}

#endif	//SPECKLE_DATABASE_BIM_RECORD
