#include "Speckle/Record/Attribute/Storey.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Database/BIMAttributeDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::attribute {
	
	class Storey::Data {
	public:
#ifdef ARCHICAD
		Data(const API_StoryType& storey) : root{storey} {}
		Data(const Data& source) : root{source.root} {}
		
		API_StoryType root;
		API_Attr_Head header;
#endif
	};
	
}

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		levelID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"level"},
	};
	
	
#ifdef ARCHICAD
	/*!
	 Fill in an Archicad API attribute header based on a storey
	 @param header The attribute header to be populated
	 @param storey The storey to be copied into the header
	 */
	void fillHeader(API_Attr_Head& header, const API_StoryType& storey) {
		active::utility::Memory::erase(header);
			//NB: This is not intended to be used for API attribute calls - it only transports core properties within this framework, e.g. name
		header.typeID = static_cast<API_AttrTypeID>(Attribute::storeyTableID);
		header.index = ACAPI_CreateAttributeIndex(storey.index);
		header.guid = Guid{Guid::fromInt(storey.floorId)};
		String{storey.uName}.writeUTF8(active::utility::BufferOut{header.name}, true);
	}
#endif

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Storey::Storey() {
} //Storey::Storey


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	index: An index into the Archicad storey array
  --------------------------------------------------------------------*/
Storey::Storey(short index) {
	m_storeyIndex = index;
} //Storey::Storey
#endif


/*--------------------------------------------------------------------
	Constructor
 
	ID: The attribute ID
  --------------------------------------------------------------------*/
Storey::Storey(const database::BIMRecordID& ID) : base{ID, storeyTableID} {
} //Storey::Storey


/*--------------------------------------------------------------------
	Copy constructor
 
 	source: The object to copy
  --------------------------------------------------------------------*/
Storey::Storey(const Storey& source) : base{source} {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
#ifdef ARCHICAD
	m_storeyIndex = source.m_storeyIndex;
#endif
} //Storey::Storey


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Storey::~Storey() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Attr_Head& Storey::getHead() const {
	confirmData();
	fillHeader(m_data->header, m_data->root);
	return m_data->header;
} //Storey::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Attr_Head& Storey::getHead() {
	confirmData();
	fillHeader(m_data->header, m_data->root);
	return m_data->header;
} //Storey::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Storey::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[levelID], levelID, element },	//TODO: implement other fields
		},
	}.withType(&typeid(Storey)));
	return base::fillInventory(inventory);
} //Storey::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Storey::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Storey))
		return base::getCargo(item);
	confirmData();
	using namespace active::serialise;
	switch (item.index) {
		case levelID:
			return std::make_unique<DoubleWrap>(m_data->root.level);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Storey::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Storey::setDefault() {

} //Storey::setDefault


/*--------------------------------------------------------------------
	Confirm the internal data, either loading from the BIM application or setting a default
  --------------------------------------------------------------------*/
void Storey::confirmData() const {
	if (m_data)
		return;
#ifdef ARCHICAD
	m_data = std::make_unique<Data>(getStoreyData());
#endif
} //Storey::confirmData


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the storey data from the host BIM application
 
	return: The storey data (for internal use to populate derived classes)
  --------------------------------------------------------------------*/
API_StoryType Storey::getStoreyData() const {
	do {
		if (auto project = addon()->getActiveProject().lock(); project) {
			auto attributeDatabase = project->getAttributeDatabase();
			if (m_storeyIndex) {
				auto storeyID = attributeDatabase->getStoreyID(*m_storeyIndex);
				m_storeyIndex.reset();
				if (!storeyID)
					break;
				resetIndex({*storeyID, Attribute::storeyTableID});
			}
			if (auto storey = attributeDatabase->getAPIStorey(getBIMLink()); storey)
				return *storey;
		}
	} while (false);
	API_StoryType storey;
	active::utility::Memory::erase(storey);
	return storey;
} //Storey::getStoreyData
#endif
