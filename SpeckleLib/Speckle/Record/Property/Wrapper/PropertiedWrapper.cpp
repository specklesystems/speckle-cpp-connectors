#include "Speckle/Record/Property/Wrapper/PropertiedWrapper.h"

#include "Speckle/Record/Property/Propertied.h"
#include "Speckle/Record/Property/Wrapper/PropertyWrapper.h"
#include "Speckle/Utility/String.h"

#include <array>
using namespace active::serialise;
using namespace speckle::record::property;
using namespace speckle::utility;

using enum Entry::Type;

namespace {
	
		///The indices of the package items
	enum FieldIndex {
		propertiesID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"Properties"},
	};

}

/*--------------------------------------------------------------------
	Constructor
 
	propertied: Reference to a propertied object
  --------------------------------------------------------------------*/
PropertiedWrapper::PropertiedWrapper(const Propertied& propertied) {
		//Serialise only properties with a defined value
	for (const auto& property : propertied.getProperties()) {
		if (property.hasDefinedValue())
			m_properties.push_back(std::reference_wrapper{const_cast<Property&>(property)});
	}
} //PropertiedWrapper::PropertiedWrapper


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool PropertiedWrapper::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[propertiesID], propertiesID, m_properties.size(), std::nullopt },
		},
	}.withType(&typeid(PropertiedWrapper)));
	return true;
} //PropertiedWrapper::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique PropertiedWrapper::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(PropertiedWrapper))
		return nullptr;
	switch (item.index) {
		case FieldIndex::propertiesID:
			if (item.available < m_properties.size())
				return std::make_unique<PropertyWrapper>(m_properties[item.available].get());
		default:
			return nullptr;	//Requested an unknown index
	}
} //PropertiedWrapper::getCargo
