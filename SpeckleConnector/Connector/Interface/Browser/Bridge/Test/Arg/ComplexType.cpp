#include "Connector/Interface/Browser/Bridge/Test/Arg/ComplexType.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		unID,
		countID,
		isTestID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"Id"},
		Identity{"count"},
		Identity{"thisIsABoolean"},
	};

}

	//Hashing specialisation
template<>
struct std::hash<connector::interfac::browser::bridge::ComplexType> {
	auto operator()(const connector::interfac::browser::bridge::ComplexType& obj) const {
		return hash<std::string>()(obj.ID) ^ rotl(hash<int32_t>()(obj.count), 1) ^ rotl(hash<bool>()(obj.testBool), 2);
	}
};


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ComplexType::ComplexType() {
		//This is the required test values
	ID = String{std::hash<ComplexType>()(*this)} + " - I am a string";
	count = static_cast<int32_t>(std::hash<ComplexType>()(*this));
} //ComplexType::ComplexType


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ComplexType::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[unID], unID, element },
			{ fieldID[countID], countID, element },
			{ fieldID[isTestID], isTestID, element },
		},
	}.withType(&typeid(ComplexType)));
	return true;
} //ComplexType::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ComplexType::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ComplexType))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case unID:
			return std::make_unique<ValueWrap<String>>(ID);
		case countID:
			return std::make_unique<ValueWrap<int32_t>>(count);
		case isTestID:
			return std::make_unique<ValueWrap<bool>>(testBool);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ComplexType::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ComplexType::setDefault() {
	ID.clear();
	count = 0;
	testBool = false;
} //ComplexType::setDefault
