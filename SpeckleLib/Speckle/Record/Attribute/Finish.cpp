#include "Speckle/Record/Attribute/Finish.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Management/Management.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Utility/BufferOut.h"
#include "Speckle/Serialise/Collection/FinishCollector.h"
#include "Speckle/Utility/Guid.h"

#ifdef ARCHICAD
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
	 */
	void copyModelerColor(const ModelerAPI::Color& modelColour, API_RGBColor& colour) {
		colour.f_red = modelColour.red;
		colour.f_green = modelColour.green;
		colour.f_blue = modelColour.blue;
	} //copyModelerColor
#endif

	int32_t ARGBToInt(double alpha, double red, double green, double blue) {
		// Convert double (0.0 - 1.0) to uint8_t (0 - 255)
		uint8_t a = static_cast<uint8_t>(std::round(alpha * 255.0));
		uint8_t r = static_cast<uint8_t>(std::round(red * 255.0));
		uint8_t g = static_cast<uint8_t>(std::round(green * 255.0));
		uint8_t b = static_cast<uint8_t>(std::round(blue * 255.0));

		// Pack ARGB into a single 32-bit integer
		return (a << 24) | (r << 16) | (g << 8) | b;
	}

	int32_t ARGBToInt(double alpha, const API_RGBColor& color) {
		// Convert double (0.0 - 1.0) to uint8_t (0 - 255)
		uint8_t a = static_cast<uint8_t>(std::round(alpha * 255.0));
		uint8_t r = static_cast<uint8_t>(std::round(color.f_red * 255.0));
		uint8_t g = static_cast<uint8_t>(std::round(color.f_green * 255.0));
		uint8_t b = static_cast<uint8_t>(std::round(color.f_blue * 255.0));

		// Pack ARGB into a single 32-bit integer
		return (a << 24) | (r << 16) | (g << 8) | b;
	}
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
Finish::Finish(const ModelerAPI::Material& material) {
	API_Attribute attr;
	active::utility::Memory::erase(attr);
	String{material.GetName()}.writeUTF8(active::utility::BufferOut{attr.header.name});
	attr.header.guid = Guid{Guid::fromInt(material.GenerateHashValue())};
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
		case diffuseID: {
			auto opacity = 1.0 - m_data->root.transpPc;
			return std::make_unique<CargoHold<Int32Wrap, int32_t>>(ARGBToInt(opacity, m_data->root.surfaceRGB));
		}			
		case opacityID:
			return std::make_unique<CargoHold<DoubleWrap,double>>(1.0);
		case emissiveID:
			return std::make_unique<CargoHold<Int32Wrap, int32_t>>(ARGBToInt(0.0, 0.0, 0.0, 0.0));
		case metalnessID:
			return std::make_unique<CargoHold<DoubleWrap, double>>(1.0);
		case roughnessID:
			return std::make_unique<CargoHold<DoubleWrap, double>>(1.0);
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
