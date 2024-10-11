#include "Connector/Record/Collection/ProjectCollection.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Record/Collection/FinishProxy.h"
#include "Speckle/Database/BIMAttributeDatabase.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Record/Element/Element.h"

#ifdef ARCHICAD
#include <ModelMaterial.hpp>
#endif

using namespace active::serialise;
using namespace connector::record;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::utility;

#ifdef ARCHICAD
namespace connector::record {
	class ProjectCollection::FinishCache : public std::unordered_map<active::utility::Guid, Finish::Unique> {};
}
#endif

namespace {

		///Serialisation fields
	enum FieldIndex {
		finishProxyID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"renderMaterialProxies"},
	};

	using WrappedProxy = CargoHold<PackageWrap, FinishProxy>;

}

/*--------------------------------------------------------------------
	Constructor
 
	project: The source project
  --------------------------------------------------------------------*/
ProjectCollection::ProjectCollection(speckle::environment::Project::Shared project) : base{project->getInfo().name, project} {
	m_finishes = std::make_unique<FinishCache>();
} //ProjectCollection::ProjectCollection


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ProjectCollection::~ProjectCollection() {
} //ProjectCollection::~ProjectCollection


/*--------------------------------------------------------------------
	Add an element to the collection hierarchy
 
	index The index of the element to add

	return: True if the element was added (false typically means the element already exists)
  --------------------------------------------------------------------*/
bool ProjectCollection::addElement(const speckle::database::BIMIndex& index) {
		//Lookup the element in the element database of the active project
	auto elementDbase = m_project->getElementDatabase();
	if (elementDbase == nullptr)
		return false;
	if (auto element = elementDbase->getElement(index); element) {
		addElement(*element);	//Add the element to the collection hierarchy
		return true;
	}
	return false;
} //ProjectCollection::addElement


/*--------------------------------------------------------------------
	Add an element to the collection hierarchy
 
	element: The element to add
 
	return: True if the element was added (false typically means the element already exists)
  --------------------------------------------------------------------*/
bool ProjectCollection::addElement(const speckle::record::element::Element& element) {
	std::vector<String> collectionNames;
		//The first collection hierarchy level is the element storey/level
	auto storey = element.getStorey();
	collectionNames.emplace_back(storey ? storey->getName() : connector()->getLocalString(titleString, noStoreyID));
		//The next level is the name of the element type
	collectionNames.emplace_back(element.getTypeName());
		//Add any future levels here as required
	RecordCollection* collection = this;
	for (const auto& childName : collectionNames)
		collection = collection->getChild(childName);
	return collection->addIndex(BIMIndex{element.getBIMID(), element.getTableID()});
} //ProjectCollection::addElement


/*--------------------------------------------------------------------
	Add a material proxy record to the collection
 
	materialIndex: The index of the material to add
	objectID: The object the material is applied to
 
	return: True if the material proxy was added (false typically means the record already exists)
  --------------------------------------------------------------------*/
bool ProjectCollection::addMaterialProxy(const speckle::database::BIMIndex& materialIndex, const speckle::database::BIMRecordID& objectID) {
	auto iter = m_finishProxies.find(materialIndex);
	if (iter == m_finishProxies.end())
		iter = m_finishProxies.insert({materialIndex, {}}).first;
	return iter->second.insert(objectID).second;
} //ProjectCollection::addMaterialProxy


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Add a ModelerAPI material to the collection (NB: These are not persistent so need to be captured by this method)
 
	material: A material
	objectID: The object the material is applied to
 
	return: True if the material proxy was added (false typically means the record already exists)
  --------------------------------------------------------------------*/
bool ProjectCollection::addMaterialProxy(const ModelerAPI::Material& material, const speckle::database::BIMRecordID& objectID) {
	auto finishID = Guid::fromInt(material.GenerateHashValue());
	auto iter = m_finishes->find(finishID);
	if (iter == m_finishes->end()) {
		auto finish = std::make_unique<Finish>(material);
		iter = m_finishes->insert({ finishID, std::move(finish) }).first;
	}
	return addMaterialProxy(finishID, objectID);
} //ProjectCollection::addMaterialProxy
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ProjectCollection::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	base::fillInventory(inventory);
	inventory.merge(Inventory{
		{
			{ Identity{fieldID[finishProxyID]}, finishProxyID, m_finishProxies.size(), std::nullopt },
		},
	}.withType(&typeid(ProjectCollection)));
	return true;
} //ProjectCollection::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ProjectCollection::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ProjectCollection))
		return base::getCargo(item);
	using namespace active::serialise;
		//TODO: This is only currently coded to write collection content - reading can be added as required in future
	switch (item.index) {
		case finishProxyID: {
			if (item.available < m_finishProxies.size()) {
				auto iter = m_finishProxies.begin();
				std::advance(iter, item.available);
				const Finish* finish = nullptr;
				if (auto fin = m_finishes->find(iter->first); fin != m_finishes->end())
					finish = fin->second.get();
				else if (auto attribute = m_project->getAttributeDatabase()->getAttribute(iter->first, iter->first.tableID); attribute)
					finish = dynamic_cast<const Finish*>(attribute.get());
				if (finish != nullptr) {
					auto proxy = std::make_unique<FinishProxy>(*finish, iter->second);
					return std::make_unique<WrappedProxy>(std::move(proxy));
				}
			}
			break;
		}
		default:
			break;
	}
	return nullptr;	//Requested an unknown index
} //ProjectCollection::getCargo
