#include "Connector/Interface/Browser/Bridge/Send/Arg/ConversionResult.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		statusID,
		srcID,
		srcTypeID,
		resID,
		resTypeID,
		errorID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"status"},
		Identity{"sourceId"},
		Identity{"sourceType"},
		Identity{"resultId"},
		Identity{"resultType"},
		Identity{"error"},
	};

		///Conversion status enumerator names
	std::array statusName{
		"success",
		"warning",
		"info",
		"error",
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ConversionResult::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[statusID], statusID, element },
			{ fieldID[srcID], srcID, element, !sourceID.empty() },
			{ fieldID[srcTypeID], srcTypeID, element, !sourceType.empty() },
			{ fieldID[resID], resID, element, !resultID.empty() },
			{ fieldID[resTypeID], resTypeID, element, !resultType.empty() },
			{ fieldID[errorID], errorID, element, error.operator bool() },
		},
	}.withType(&typeid(ConversionResult)));
	return true;
} //ConversionResult::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ConversionResult::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(ConversionResult))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case statusID:
			return Cargo::Unique{new CargoHold<ValueWrap<uint16_t>, uint16_t>(static_cast<uint16_t>(status))};
		case srcID:
			return std::make_unique<ValueWrap<String>>(sourceID);
		case srcTypeID:
			return std::make_unique<ValueWrap<String>>(sourceType);
		case resID:
			return std::make_unique<ValueWrap<String>>(resultID);
		case resTypeID:
			return std::make_unique<ValueWrap<String>>(resultType);
		case errorID:
			return std::make_unique<PackageWrap>(*error);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ConversionResult::getCargo


/*--------------------------------------------------------------------
	Get a conversion status enumerator from text
 
	text; The incoming text
 
	return: The equivalent conversion status (nullopt on failure)
  --------------------------------------------------------------------*/
std::optional<ConversionResult::Status> connector::interfac::browser::bridge::toConversionStatus(const String& text) {
	for (auto i = 0; i < statusName.size(); ++i)
		if (text == statusName[i])
			return static_cast<ConversionResult::Status>(i + 1);
	return std::nullopt;
} //active::geometry::toAnchor2D


/*--------------------------------------------------------------------
	Get the text for a conversion status value
 
	status: The incoming status
 
	return: The conversion status as text
  --------------------------------------------------------------------*/
String connector::interfac::browser::bridge::fromConversionStatus(ConversionResult::Status status) {
	return statusName.at(static_cast<size_t>(status) - 1);
} //active::geometry::fromAnchor2D
