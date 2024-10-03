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
		 @param ID The record ID
		 @param globID The global ID
		 */
		Record(speckle::utility::String::Option ID = std::nullopt, speckle::utility::String::Option globID = std::nullopt) :
				base{ID.value_or(active::utility::Guid{true}.operator active::utility::String()),
					globID.value_or(active::utility::Guid{true}.operator active::utility::String())} {}
		/*!
		 Destructor
		 */
		virtual ~Record() {}
		
		// MARK: - Functions (const)
		
		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required, but "Base" is still considered a type on its own)
		 */
		virtual speckle::utility::String getSpeckleType() const { return "Base"; }
		
		// MARK: - Functions (mutating)

		
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
			///Cache for the speckle type during serialisation operations
		mutable speckle::utility::String::Option m_type;
	};
	
}

#endif	//SPECKLE_DATABASE_RECORD
