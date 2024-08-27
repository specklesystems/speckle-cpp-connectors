#ifndef CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_CONFIG

#include "Active/Serialise/Package/Package.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 Configuration settings class
	 */
	class ConnectorConfig : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ConnectorConfig() = default;
		
		// MARK: - Public variables (NB: Assuming to class invariants or overrides for this data, so making public for simplicity)
		
			///?
		bool isDarkTheme = true;
		
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
		void setDefault() override { isDarkTheme = true; }
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_CONFIG
