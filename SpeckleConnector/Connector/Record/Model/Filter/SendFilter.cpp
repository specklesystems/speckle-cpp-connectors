#include "Connector/Record/Model/Filter/SendFilter.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"

#include <array>

using namespace active::serialise;
using namespace speckle::database;
using namespace connector::record;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		nameID,
		summaryID,
		defaultID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
		Identity{"summary"},
		Identity{"isDefault"},
	};

}

/*--------------------------------------------------------------------
	Determine if the filter has expired because an element in the selection has changed
 
	changed: The list of changed element IDs
 
	return: True if the one of the changed elements is in the selection
  --------------------------------------------------------------------*/
bool SendFilter::checkExpiry(const ElementIDList& changed) const {
	ElementIDList intersect;
	ElementIDList mine{getElementIDs()}, theirs{changed};
	std::sort(mine.begin(), mine.end());
	std::sort(theirs.begin(), theirs.end());
	std::set_intersection (mine.begin(), mine.end(), theirs.begin(), theirs.end(), std::back_inserter(intersect));
	return !intersect.empty();
} //SendFilter::checkExpiry


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SendFilter::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
			{ fieldID[summaryID], summaryID, element, !m_summary.empty() },
			{ fieldID[defaultID], defaultID, element, m_isDefault },
		},
	}.withType(&typeid(SendFilter)));
	return true;
} //SendFilter::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SendFilter::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(SendFilter))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			return std::make_unique<ValueWrap<String>>(m_name);
		case summaryID:
			return std::make_unique<ValueWrap<String>>(m_summary);
		case defaultID:
			return std::make_unique<ValueWrap<bool>>(m_isDefault);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendFilter::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void SendFilter::setDefault() {
	m_name.clear();
	m_summary.clear();
	m_isDefault = false;
} //SendFilter::setDefault
