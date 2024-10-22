#ifndef SPECKLE_RECORD_PROPERTY_PROPERTIED_WRAPPER
#define SPECKLE_RECORD_PROPERTY_PROPERTIED_WRAPPER

#include "Active/Serialise/Package/Package.h"
#include "Active/Utility/String.h"


namespace speckle::record::property {

	class Property;
	class Propertied;
	
	/*!
		Wrapper for (de)seriliasing the contents of the Archicad Properties and Classification tab
	*/
	class PropertiedWrapper : public active::serialise::Package {
	public:
			
		// MARK: - Constructor

		/*!
			Constructor
			@param propertied Reference to a propertied object
		*/
		PropertiedWrapper(const Propertied& propertied);
		
		// MARK: - Functions (serialisation)
		
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
		
	private:
		using PropertyList = std::vector<std::reference_wrapper<Property>>;
			//The wrapped properties
		PropertyList m_properties;
	};
	
}

#endif	//SPECKLE_RECORD_PROPERTY_PROPERTIED_WRAPPER
