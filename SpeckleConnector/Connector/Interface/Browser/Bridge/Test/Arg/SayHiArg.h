#ifndef CONNECTOR_INTERFACE_BRIDGE_SAYHI_ARG
#define CONNECTOR_INTERFACE_BRIDGE_SAYHI_ARG

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 Object for testing JS comms (with TestBridge binding)
	 */
	class SayHiArg final : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SayHiArg() = default;
		
		// MARK: - Public variables (NB: Assuming to class invariants or overrides for this data, so making public for simplicity)
		
			///Sample parameters for testing only
		speckle::utility::String name;
		int32_t count = 0;
		bool isHello = false;
		
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

#endif	//CONNECTOR_INTERFACE_BRIDGE_SAYHI_ARG
