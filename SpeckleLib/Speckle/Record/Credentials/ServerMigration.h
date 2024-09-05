#ifndef SPECKLE_RECORD_CRED_SERVER_MIGRATION
#define SPECKLE_RECORD_CRED_SERVER_MIGRATION

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::cred {
	
	/*!
	 User account record class
	 */
	class ServerMigration : public active::serialise::Package {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param mvTo New URI where this server is now deployed
		 @param mvFm Previous URI where this server used to be deployed
		 */
		ServerMigration(const speckle::utility::String& mvTo = speckle::utility::String{},
						const speckle::utility::String& mvFm = speckle::utility::String{}) : movedTo{mvTo}, movedFrom{mvFm} {}

		// MARK: - Public variables
		 
			///New URI where this server is now deployed
		speckle::utility::String movedTo;
			///Previous URI where this server used to be deployed
		speckle::utility::String movedFrom;
		
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
	};

}

#endif	//SPECKLE_RECORD_CRED_SERVER_MIGRATION
