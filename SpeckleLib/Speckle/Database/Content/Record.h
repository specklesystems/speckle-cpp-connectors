#ifndef SPECKLE_DATABASE_RECORD
#define SPECKLE_DATABASE_RECORD

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Database/Content/Link.h"

namespace speckle::database {

	/*!
	 Base class for a database record
	 */
	class Record : public active::serialise::Package, public active::utility::Cloner {
	public:

		// MARK: - Types
		
		using Unique = std::unique_ptr<Record>;
			///Shared pointer
		using Shared = std::shared_ptr<Record>;
			///Optional
		using Option = std::optional<Record>;

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param ID The record ID
		 */
		Record(speckle::utility::Guid ID) : m_ID{ID} {}
		/*!
		 Destructor
		 */
		virtual ~Record() {}
		
		// MARK: - Functions (const)
		
		/*!
		 Get the record ID
		 @return The record ID
		 */
		speckle::utility::Guid getID() const { return m_ID; }
		/*!
		 Get the record unique ID
		 @return The record unique ID
		 */
		speckle::utility::Guid getUniqueID() const { return m_uniqueID; }
		/*!
		 Get the record index
		 @return The record index
		 */
		virtual Index getIndex() const { return Index{m_ID, m_ownerID}; }
		/*!
		 Get the record link
		 @return The record link
		 */
		virtual Link getLink() const { return Link{getIndex()}; }
		
		// MARK: - Functions (mutating)
		
		/*!
		 Set the record ID
		 @param ID The record ID
		 */
		void setID(speckle::utility::Guid ID) { m_ID = ID; }
		/*!
		 Set the record unique ID
		 @param ID The record unique ID
		 */
		void setUniqueID(speckle::utility::Guid ID) { m_uniqueID = ID; }
		
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
		/*!
			Validate the cargo data
			@return True if the data has been validated
		*/
		bool validate() override;
		
	private:
			///The record ID (may not be unique across models)
		speckle::utility::Guid m_ID = speckle::utility::Guid{true};
			///The record unique ID
		speckle::utility::Guid m_uniqueID = speckle::utility::Guid{true};
			///The record owner ID (null guid if unspecified)
		speckle::utility::Guid m_ownerID;
			///The last edit time
		active::utility::Time editTime = active::utility::Time{};
	};
	
}

#endif	//SPECKLE_DATABASE_RECORD
