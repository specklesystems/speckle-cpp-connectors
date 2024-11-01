#include "Speckle/Record/Attribute/Material.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Management/Management.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Serialise/Collection/FinishCollector.h"
#include "Speckle/Serialise/Types/ArchicadRGB.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::serialise;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::attribute {
	
		///Internal representation of a rendered finish on a 3D body, i.e. the surface colour/texture etc.
	class Material::Data {
	public:
#ifdef ARCHICAD
		/*!
		 Constructor from Archicad material
		 @param attr An Archicad attribute
		 */
		Data(const API_Attribute& attr) : root{attr.buildingMaterial} {
		}
		
			///Archicad representation of a building material
		API_BuildingMaterialType root;
#endif
	};
	
}

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		thermConductID,
			//TODO: Add other fields as required
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"thermalConductivity"},
			//TODO: Add other fields as required
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Material::Material() {
} //Material::Material


/*--------------------------------------------------------------------
	Constructor
 
	ID: The attribute ID
  --------------------------------------------------------------------*/
Material::Material(const database::BIMRecordID& ID) : base{ID, Material::table} {
} //Material::Material


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	attrData: Archicad attribute data
	tableID: The ID of the parent table
  --------------------------------------------------------------------*/
Material::Material(const API_Attribute& attrData, const BIMRecordID& tableID) : base{attrData.header.guid, Material::table} {
	m_data = std::make_unique<Data>(attrData);
	setUnit(std::nullopt);	//Finishes have no unit
}
#endif


/*--------------------------------------------------------------------
	Copy constructor
 
 	source: The object to copy
  --------------------------------------------------------------------*/
Material::Material(const Material& source) : base{source} {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Material::Material


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Material::~Material() {}


/*--------------------------------------------------------------------
	Assignment operator
 
	source: The object to assign
 
	return: A reference to this
  --------------------------------------------------------------------*/
Material& Material::operator=(const Material& source) {
	if (this != &source) {
		base::operator=(source);
		m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
	}
	return *this;
} //Material::operator=


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Attr_Head& Material::getHead() const {
	confirmData();
	return m_data->root.head;
} //Material::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API attribute header data
 
	return: The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Attr_Head& Material::getHead() {
	confirmData();
	return m_data->root.head;
} //Material::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Material::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[thermConductID], thermConductID, element },
		},
	}.withType(&typeid(Material)));
	return base::fillInventory(inventory);
} //Material::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Material::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Material))
		return base::getCargo(item);
	confirmData();
	using namespace active::serialise;
	switch (item.index) {
		case thermConductID:
			return std::make_unique<DoubleWrap>(m_data->root.thermalConductivity);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Material::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Material::setDefault() {
	m_data->root = {};
} //Material::setDefault


/*--------------------------------------------------------------------
	Confirm the internal data, either loading from the BIM application or setting a default
  --------------------------------------------------------------------*/
void Material::confirmData() const {
	if (m_data)
		return;
	m_data = std::make_unique<Data>(getData());
} //Material::confirmData
