#include "Connector/Record/Collection/RecordCollection.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Speckle/Database/BIMElementDatabase.h"

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>

namespace {

		///Serialisation fields
	enum FieldIndex {
		nameID,
		elementID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
		Identity{"elements"},
	};

	using WrappedElement = CargoHold<PackageWrap, Element>;

}

/*--------------------------------------------------------------------
	Constructor
 
	name: The collection name
	project: The source project
  --------------------------------------------------------------------*/
RecordCollection::RecordCollection(const speckle::utility::String& name, Project::Shared project) : m_name{name}, m_project{project} {
} //RecordCollection::RecordCollection


/*--------------------------------------------------------------------
	Get a child collection by name (adding if missing)
 
	name: The child name
 
	return: A pointer to the requested child (nullptr on failure, caller does not take ownership)
  --------------------------------------------------------------------*/
RecordCollection* RecordCollection::getChild(const speckle::utility::String& name) {
		//Return an existing child if possible
	if (auto iter = m_children.find(name); iter != m_children.end())
		return &iter->second;
		//Otherwise insert and return a new collection with the requested name
	return &m_children.insert({name, RecordCollection{name, m_project}}).first->second;
} //RecordCollection::getChild


/*--------------------------------------------------------------------
	Add an index to the collection
 
	index: The index to add
 
	return: True if the index was added (false typically means the index already exists)
  --------------------------------------------------------------------*/
bool RecordCollection::addIndex(const speckle::database::BIMIndex& index) {
	return m_indices.insert(index).second;
} //RecordCollection::addIndex


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool RecordCollection::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	base::fillInventory(inventory);
	inventory.merge(Inventory{
		{
			{ Identity{fieldID[nameID]}, nameID, element },
			{ Identity{fieldID[elementID]}, elementID, m_children.size() + m_indices.size(), std::nullopt },
		},
	}.withType(&typeid(RecordCollection)));
	return true;
} //RecordCollection::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique RecordCollection::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(RecordCollection))
		return base::getCargo(item);
	using namespace active::serialise;
		//TODO: This is only currently coded to write collection content - reading can be added as required in future
	switch (item.index) {
		case nameID:
			return std::make_unique<StringWrap>(m_name);
		case elementID: {
			if (item.available < m_children.size()) {
				auto iter = m_children.begin();
				std::advance(iter, item.available);
				return std::make_unique<PackageWrap>(iter->second);
			}
			auto index = item.available - m_children.size();
			if (index < m_indices.size()) {
				auto iter = m_indices.begin();
				std::advance(iter, index);
				if (auto element = m_project->getElementDatabase()->getElement(*iter, iter->tableID); element)
					return std::make_unique<WrappedElement>(std::move(element));
			}
			break;
		}
		default:
			break;
	}
	return nullptr;	//Requested an unknown index
} //RecordCollection::getCargo
