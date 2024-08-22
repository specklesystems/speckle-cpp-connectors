#ifndef CONNECTOR_DATABASE_MODEL_CARD_DATABASE
#define CONNECTOR_DATABASE_MODEL_CARD_DATABASE

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace connector::database {
	
	/*!
	 Database of model cards relating to a specific project
	 */
	class ModelCardDatabase : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ModelCardDatabase() {}
		
		// MARK: - Public variables (NB: Assuming to class invariants or overrides for this data, so making public for simplicity)
		
		
		// MARK: Functions (const)
		
			//Implement card access when necessary
		
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
		//TODO: Model card array here - implement when necessary
	};

}

#endif	//CONNECTOR_DATABASE_MODEL_CARD_DATABASE
