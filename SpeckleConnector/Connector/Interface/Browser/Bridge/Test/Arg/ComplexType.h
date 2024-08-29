#ifndef CONNECTOR_INTERFACE_BRIDGE_COMPLEX_TYPE
#define CONNECTOR_INTERFACE_BRIDGE_COMPLEX_TYPE

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 Object for testing Javascript communication (with TestBridge binding)
	 */
	class ComplexType final : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ComplexType();
		
		// MARK: - Public variables
		
			///Sample parameters for testing only
		speckle::utility::String ID;
		int32_t count = 0;
		bool testBool = false;

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

#endif	//CONNECTOR_INTERFACE_BRIDGE_COMPLEX_TYPE
