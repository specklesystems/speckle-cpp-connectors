#ifndef SPECKLE_RECORD_PROPERTY_CLASS_WRAPPER
#define SPECKLE_RECORD_PROPERTY_CLASS_WRAPPER

#include "Active/Serialise/Package/Package.h"
#include "Active/Utility/String.h"

namespace speckle::record::property {

	class Propertied;
	
	/*!
		Wrapper for (de)seriliasing the contents of the Archicad Properties and Classification tab
	*/
	class PropsAndClassWrapper : public active::serialise::Package {
	public:
			
		// MARK: - Constructor

		/*!
			Constructor
			@param propertied Reference to a propertied object
		*/
		PropsAndClassWrapper(const Propertied& propertied);
		
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
			//Wrapper for the Properties section
		std::reference_wrapper<Propertied> m_property;
	};
	
}

#endif	//SPECKLE_RECORD_PROPERTY_CLASS_WRAPPER
