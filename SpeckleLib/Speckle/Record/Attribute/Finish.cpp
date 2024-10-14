#include "Speckle/Record/Attribute/Finish.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Management/Management.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Serialise/Collection/FinishCollector.h"
#include "Speckle/Serialise/Types/ArchicadRGB.h"
#include "Speckle/Utility/Guid.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <ModelMaterial.hpp>
#endif

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record::attribute;
using namespace speckle::serialise;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::attribute {
	
		///Internal representation of a rendered finish on a 3D body, i.e. the surface colour/texture etc.
	class Finish::Data {
	public:
#ifdef ARCHICAD
		/*!
		 Constructor from Archicad surface material
		 @param attr An Archicad attribute
		 */
		Data(const API_Attribute& attr) : root{attr.material} {
			opacity = 1.0 - (static_cast<double>(attr.material.transpPc) / 100.0);
			roughness = 1.0 - (static_cast<double>(attr.material.shine) / 10000.0);
		}
		
			///Archicad representation of a surface material
		API_MaterialType root;
#endif
			//Opacity (0.0 -> 1.0)
		double opacity = 1.0;
			//Roughness (0.0 -> 1.0)
		double roughness = 0.0;
			//Metalness (0.0 -> 1.0)
		double metalness = 0.0;
	};
	
}

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		diffuseID,
		opacityID,
		emissiveID,
		metalnessID,
		roughnessID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"diffuse"},
		Identity{"opacity"},
		Identity{"emissive"},
		Identity{"metalness"},
		Identity{"roughness"},
	};

#ifdef ARCHICAD
	/*!
	 Copy a ModelerAPI colour to an AC RGB colour
	 @param modelColour The modeler API colour
	 @param colour The attribute API colour
	 */
	void copyModelerColor(const ModelerAPI::Color& modelColour, API_RGBColor& colour) {
		colour.f_red = modelColour.red;
		colour.f_green = modelColour.green;
		colour.f_blue = modelColour.blue;
	}
#endif
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


/*--------------------------------------------------------------------
	Constructor
 
	material: A ModelerAPI material definition
  --------------------------------------------------------------------*/
Finish::Finish(const ModelerAPI::Material& material) : base{Guid{Guid::fromInt(material.GenerateHashValue())}, Finish::table} {
	API_Attribute attr;
	active::utility::Memory::erase(attr);
	String{material.GetName()}.writeUTF8(active::utility::BufferOut{attr.header.name});
	attr.header.guid = getBIMID();
	attr.material.mtype = static_cast<API_MaterTypeID>(material.GetType());
	attr.material.ambientPc = static_cast<short>(material.GetAmbientReflection() * 100);
	attr.material.diffusePc = static_cast<short>(material.GetDiffuseReflection() * 100);
	attr.material.specularPc = static_cast<short>(material.GetSpecularReflection() * 100);
	attr.material.transpPc = static_cast<short>(material.GetTransparency() * 100);
	attr.material.shine = static_cast<short>(material.GetShining() * 10000);
	attr.material.transpAtt = static_cast<short>(material.GetTransparencyAttenuation() * 400);
	attr.material.emissionAtt = static_cast<short>(material.GetEmissionAttenuation() * 65535);
	copyModelerColor(material.GetSurfaceColor(), attr.material.surfaceRGB);
	copyModelerColor(material.GetSpecularColor(), attr.material.specularRGB);
	copyModelerColor(material.GetEmissionColor(), attr.material.emissionRGB);
	m_data = std::make_unique<Data>(attr);
} //Finish::Finish
#endif


/*--------------------------------------------------------------------
	Copy constructor
 
 	source: The object to copy
  --------------------------------------------------------------------*/
Finish::Finish(const Finish& source) : base{source} {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
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
			{ fieldID[diffuseID], diffuseID, element },
			{ fieldID[opacityID], opacityID, element },
			{ fieldID[emissiveID], emissiveID, element },
			{ fieldID[metalnessID], metalnessID, element },
			{ fieldID[roughnessID], roughnessID, element },
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
		case diffuseID:
#ifdef ARCHICAD
			return std::make_unique<ValueWrap<API_RGBColor>>(m_data->root.surfaceRGB);
#endif
		case opacityID:
			return std::make_unique<DoubleWrap>(m_data->opacity);
		case emissiveID:
#ifdef ARCHICAD
			return std::make_unique<ValueWrap<API_RGBColor>>(m_data->root.emissionRGB);
#endif
		case metalnessID:
			return std::make_unique<DoubleWrap>(m_data->metalness);
		case roughnessID:
			return std::make_unique<DoubleWrap>(m_data->roughness);
		default:
			return nullptr;	//Requested an unknown index
	}
} //Finish::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Finish::setDefault() {
	m_data->root = {};
	m_data->opacity = 0.0;
	m_data->roughness = 1.0;
	m_data->metalness = 0.0;
} //Finish::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool Finish::validate() {
	m_data->root.transpPc = static_cast<short>(100 * (1.0 - m_data->opacity));
	m_data->root.shine = static_cast<short>(10000 * (1.0 - m_data->roughness));
		//NB: Archicad has no metalness value - currently discarded
	return true;
} //Finish::validate


/*--------------------------------------------------------------------
	Confirm the internal data, either loading from the BIM application or setting a default
  --------------------------------------------------------------------*/
void Finish::confirmData() const {
	if (m_data)
		return;
	m_data = std::make_unique<Data>(getData());
} //Finish::confirmData
