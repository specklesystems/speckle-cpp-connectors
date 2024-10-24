#include "Connector/Record/Model/ReceiverModelCard.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"

#include <array>

using namespace active::serialise;
using namespace speckle::database;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		projectNameID,
		modelNameID,
		selectedVersionID,
		latestVersionID,
		warningDismissedID,
		bakedObjectsID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"projectName"},
		Identity{"modelName"},
		Identity{"selectedVersionID"},
		Identity{"latestVersionID"},
		Identity{"hasDismissedUpdateWarning"},
		Identity{"bakedObjectIds"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ReceiverModelCard::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[projectNameID], projectNameID, element },
			{ fieldID[modelNameID], modelNameID, element },
			{ fieldID[selectedVersionID], selectedVersionID, element },
			{ fieldID[latestVersionID], latestVersionID, element },
			{ fieldID[warningDismissedID], warningDismissedID, element },
			{ fieldID[bakedObjectsID], bakedObjectsID, element },
		},
	}.withType(&typeid(ReceiverModelCard)));
	return base::fillInventory(inventory);
} //ReceiverModelCard::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ReceiverModelCard::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ReceiverModelCard))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case projectNameID:
			return std::make_unique<StringWrap>(m_projectName);
		case modelNameID:
			return std::make_unique<StringWrap>(m_modelName);
		case selectedVersionID:
			return std::make_unique<ValueWrap<RecordID>>(m_selectedVersionID);
		case latestVersionID:
			return std::make_unique<ValueWrap<RecordID>>(m_latestVersionID);
		case warningDismissedID:
			return std::make_unique<BoolWrap>(m_hasDismissedUpdateWarning);
		case bakedObjectsID:
			return std::make_unique<ContainerWrap<ElementIDList>>(m_bakedObjectIDs);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ReceiverModelCard::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ReceiverModelCard::setDefault() {
	base::setDefault();
	m_projectName.clear();
	m_modelName.clear();
	m_selectedVersionID.clear();
	m_latestVersionID.clear();
	m_hasDismissedUpdateWarning = false;
	m_bakedObjectIDs.clear();
} //ReceiverModelCard::setDefault
