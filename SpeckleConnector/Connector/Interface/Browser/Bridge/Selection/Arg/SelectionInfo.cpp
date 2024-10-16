#include "Connector/Interface/Browser/Bridge/Selection/Arg/SelectionInfo.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

#include "Connector/Connector.h"

#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Element/Element.h"
using namespace speckle::record::element;

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;

namespace {

		///Serialisation fields
	enum FieldIndex {
		selectedObjectIdsID,
		summaryID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"selectedObjectIds"},
		Identity{"summary"},
	};

}

SelectionInfo::SelectionInfo() {
	initialize();
}

void SelectionInfo::initialize() {
	auto project = connector()->getActiveProject().lock();
	if (!project) {
		// TODO: handle
	}

	auto elementDatabase = project->getElementDatabase();
	auto selected = elementDatabase->getSelection();

	active::utility::String summary(selected.size());
	summary += " objects selected.";
	m_summary = summary;

	for (const auto& link : selected) {
		m_selectedElementIds.push_back(link);
	}
}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SelectionInfo::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[selectedObjectIdsID], selectedObjectIdsID, element },
			{ fieldID[summaryID], summaryID, element },
		},
	}.withType(&typeid(SelectionInfo)));
	return true;
} //ConnectorConfig::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SelectionInfo::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(SelectionInfo))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case selectedObjectIdsID:
			return std::make_unique<ContainerWrap<std::vector<active::utility::Guid>>>(m_selectedElementIds);
		case summaryID:
			return std::make_unique<ValueWrap<active::utility::String>>(m_summary);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ConnectorConfig::getCargo
