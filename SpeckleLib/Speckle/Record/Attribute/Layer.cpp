#include "Speckle/Record/Attribute/Layer.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Management/Management.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Serialise/Types/ArchicadRGB.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::attribute {
	
		///Internal representation of a rendered layer on a 3D body, i.e. the surface colour/texture etc.
	class Layer::Data {
	public:
#ifdef ARCHICAD
		/*!
		 Constructor from Archicad surface material
		 @param attr An Archicad attribute
		 */
		Data(const API_Attribute& attr) : root{attr.layer} {
			isHidden = root.head.flags & APILay_Hidden;
			isLocked = root.head.flags & APILay_Locked;
		}
		
			///Archicad representation of a layer
		API_LayerType root;
#endif
			///True if the layer is hidden
		bool isHidden = false;
			///True if the layer is locked
		bool isLocked = false;
	};
	
}

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		hiddenID,
		lockedID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"hidden"},
		Identity{"locked"},
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Layer::Layer() {
} //Layer::Layer


/*--------------------------------------------------------------------
	Constructor
 
	ID: The attribute ID
  --------------------------------------------------------------------*/
Layer::Layer(const database::BIMRecordID& ID) : base{ID, Layer::table} {
} //Layer::Layer


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	attrData: Archicad attribute data
	tableID: The ID of the parent table
  --------------------------------------------------------------------*/
Layer::Layer(const API_Attribute& attrData, const BIMRecordID& tableID) : base{attrData.header.guid, Layer::table} {
	m_data = std::make_unique<Data>(attrData);
	setUnit(std::nullopt);	//Layeres have no unit
}
#endif


/*--------------------------------------------------------------------
	Copy constructor
 
 	source: The object to copy
  --------------------------------------------------------------------*/
Layer::Layer(const Layer& source) : base{source} {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Layer::Layer


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Layer::~Layer() {}


/*--------------------------------------------------------------------
	Assignment operator
 
	source: The object to assign
 
	return: A reference to this
  --------------------------------------------------------------------*/
Layer& Layer::operator=(const Layer& source) {
	if (this != &source) {
		base::operator=(source);
		m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
	}
	return *this;
} //Layer::operator=


/*--------------------------------------------------------------------
	Determine if the layer is hidden
 
	return: True if the layer is hidden
  --------------------------------------------------------------------*/
bool Layer::isHidden() const {
	confirmData();
	return m_data->isHidden;
} //Layer::isHidden


/*--------------------------------------------------------------------
	Determine if the layer is locked
 
	return: True if the layer is locked
  --------------------------------------------------------------------*/
bool Layer::isLocked() const {
	confirmData();
	return m_data->isLocked;
} //Layer::isLocked


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Attr_Head& Layer::getHead() const {
	confirmData();
	return m_data->root.head;
} //Layer::getHead
#endif


/*--------------------------------------------------------------------
	Set whether the layer is hidden
 
	state: True if the layer is hidden
  --------------------------------------------------------------------*/
void Layer::setHidden(bool state) {
	confirmData();
	m_data->isHidden = state;
#ifdef ARCHICAD
	if (state)
		m_data->root.head.flags |= APILay_Hidden;
	else
		m_data->root.head.flags &= ~APILay_Hidden;
#endif
} //Layer::setHidden


/*--------------------------------------------------------------------
	Set whether the layer is locked
 
	state: True if the layer is locked
  --------------------------------------------------------------------*/
void Layer::setLocked(bool state) {
	confirmData();
	m_data->isLocked = state;
#ifdef ARCHICAD
	if (state)
		m_data->root.head.flags |= APILay_Locked;
	else
		m_data->root.head.flags &= ~APILay_Locked;
#endif
} //Layer::setLocked


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (mutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Attr_Head& Layer::getHead() {
	confirmData();
	return m_data->root.head;
} //Layer::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Layer::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[hiddenID], hiddenID, element },
			{ fieldID[lockedID], lockedID, element },
		},
	}.withType(&typeid(Layer)));
	return base::fillInventory(inventory);
} //Layer::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Layer::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Layer))
		return base::getCargo(item);
	confirmData();
	using namespace active::serialise;
	switch (item.index) {
		case hiddenID:
			return std::make_unique<BoolWrap>(m_data->isHidden);
		case lockedID:
			return std::make_unique<BoolWrap>(m_data->isLocked);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Layer::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Layer::setDefault() {
	m_data->root = {};
	m_data->isHidden = false;
	m_data->isLocked = false;
} //Layer::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool Layer::validate() {
#ifdef ARCHICAD
	if (m_data->isHidden)
		m_data->root.head.flags |= APILay_Hidden;
	if (m_data->isLocked)
		m_data->root.head.flags |= APILay_Locked;
#endif
	return true;
} //Layer::validate


/*--------------------------------------------------------------------
	Get the attribute data to be written to the database

	return: The attribute data (for internal use to write to the database)
  --------------------------------------------------------------------*/
API_Attribute Layer::getDataOut() const {
	confirmData();
	API_Attribute result;
	active::utility::Memory::erase(result);
	result.layer = m_data->root;
	return result;
} //Layer::getDataOut


/*--------------------------------------------------------------------
	Confirm the internal data, either loading from the BIM application or setting a default
  --------------------------------------------------------------------*/
void Layer::confirmData() const {
	if (m_data)
		return;
	m_data = std::make_unique<Data>(getData());
} //Layer::confirmData
