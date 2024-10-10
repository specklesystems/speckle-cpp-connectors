#include "Speckle/Record/Attribute/Finish.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::attribute {
	
	class Finish::Data {
	public:
#ifdef ARCHICAD
		Data(const API_Attribute& attr) : root{attr.material} {}
		Data(const Data& source) : root{source.root} {}
		
		API_MaterialType root;
#endif
	};
	
}

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		surfaceColourID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"surfaceColour"},
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Finish::Finish() {
} //Finish::Finish


/*--------------------------------------------------------------------
	Constructor
 
	ID: The attribute ID
  --------------------------------------------------------------------*/
Finish::Finish(const database::BIMRecordID& ID) : base{ID, Finish::table} {
} //Finish::Finish


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	attrData: Archicad attribute data
	tableID: The ID of the parent table
  --------------------------------------------------------------------*/
Finish::Finish(const API_Attribute& attrData, const BIMRecordID& tableID) : base{attrData.header.guid, Finish::table} {
	m_data = std::make_unique<Data>(attrData);
}
#endif


/*--------------------------------------------------------------------
	Copy constructor
 
 	source: The object to copy
  --------------------------------------------------------------------*/
Finish::Finish(const Finish& source) : base{source} {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Finish::Finish


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Finish::~Finish() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Attr_Head& Finish::getHead() const {
	confirmData();
	return m_data->root.head;
} //Finish::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Attr_Head& Finish::getHead() {
	confirmData();
	return m_data->root.head;
} //Finish::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Finish::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[surfaceColourID], surfaceColourID, element },	//TODO: implement other fields
		},
	}.withType(&typeid(Finish)));
	return base::fillInventory(inventory);
} //Finish::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Finish::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Finish))
		return base::getCargo(item);
	confirmData();
	using namespace active::serialise;
	switch (item.index) {
		case surfaceColourID:
			return nullptr;	//TODO: lookup surface colour
		default:
			return nullptr;	//Requested an unknown index
	}
} //Finish::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Finish::setDefault() {

} //Finish::setDefault


/*--------------------------------------------------------------------
	Confirm the internal data, either loading from the BIM application or setting a default
  --------------------------------------------------------------------*/
void Finish::confirmData() const {
	if (m_data)
		return;
	m_data = std::make_unique<Data>(getData());
} //Finish::confirmData
