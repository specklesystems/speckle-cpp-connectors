#ifndef CONNECTOR_INTERFACE_BRIDGE_SELECTION_INFO
#define CONNECTOR_INTERFACE_BRIDGE_SELECTION_INFO

#include "Active/Serialise/Package/Package.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 Configuration settings class
	 */
	class SelectionInfo : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SelectionInfo();
		
		active::utility::String m_summary = "No objects selected";
		std::vector<active::utility::Guid> m_selectedElementIds;

		void initialize();
		
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
	};
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SELECTION_INFO
