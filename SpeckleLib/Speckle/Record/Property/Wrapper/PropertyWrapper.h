#ifndef SPECKLE_RECORD_PROPERTY_WRAPPER
#define SPECKLE_RECORD_PROPERTY_WRAPPER

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::property {
	
	class Property;

	/*!
		Wrapper for (de)seriliasing the a property
	*/
	class PropertyWrapper : public active::serialise::Package {
	public:
			
		// MARK: - Constructor

		/*!
			Constructor
			@param property Reference to a property object
		*/
		PropertyWrapper(const Property& property);
		
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
		/*!
			Set to the default package content
		*/
		void setDefault() override;
		
		//TODO: Add validation when we need to deserialise properties
		
	private:
			//The property name
		utility::String m_name;
			//The property group name
		utility::String m_groupName;
			//The property value (as displayed)
		utility::String m_displayValue;
			//The property (template) ID
		utility::Guid m_ID;
	};
	
}

#endif	//SPECKLE_RECORD_PROPERTY_WRAPPER
