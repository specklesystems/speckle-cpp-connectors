#include "Speckle/Record/Property/Wrapper/PropsAndClassWrapper.h"

#include "Speckle/Record/Property/Wrapper/PropertiedWrapper.h"
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
		classificationID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"Classification"},
		Identity{"Properties"},
	};

}  // namespace

// MARK: - Constructors

/*--------------------------------------------------------------------
	Constructor
 
	point: The point to wrap for (de)serialisation
  --------------------------------------------------------------------*/
PropsAndClassWrapper::PropsAndClassWrapper(const Propertied& propertied) : m_property(const_cast<Propertied&>(propertied)) {}


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool PropsAndClassWrapper::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[propertiesID], propertiesID, element },
				//TODO: Implement object classifications
			//{ fieldID[classificationID], classificationID, element },
		},
	}.withType(&typeid(PropsAndClassWrapper)));
	return true;
} //PropsAndClassWrapper::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique PropsAndClassWrapper::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(PropsAndClassWrapper))
		return nullptr;
	switch (item.index) {
		case FieldIndex::propertiesID:
			return std::make_unique<PropertiedWrapper>(m_property.get());
		case FieldIndex::classificationID:
			return nullptr; //TODO: Implement object classifications
		default:
			return nullptr;	//Requested an unknown index
	}
} //PropsAndClassWrapper::getCargo
