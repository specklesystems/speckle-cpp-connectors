#include "Speckle/Record/Element/Roof.h"

#include "Speckle/Record/Element/Quants/Surveyor.h"
#include "Speckle/Record/Element/Setting/Conversion.h"
#include "Speckle/Utility/BIMMemory.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::record::element::quants;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class Roof::Data {
	public:
		friend class Roof;

#ifdef ARCHICAD
		Data(const API_RoofType& seg) : root{seg} {}

	private:
		API_RoofType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Roof::Roof() {
} //Roof::Roof


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Roof::Roof(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.roof);
} //Roof::Roof
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Roof::Roof(const Roof& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Roof::Roof


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Roof::Roof(Roof&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //Roof::Roof


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Roof::~Roof() {}


/*--------------------------------------------------------------------
	Get the composition of materials in the element
 
	return: The material composition (element with ordered material composition should override)
  --------------------------------------------------------------------*/
ModelElement::Composition Roof::getComposition() const {
#ifdef ARCHICAD
	return convert(m_data->root.shellBase.modelElemStructureType);
#endif
} //Roof::getComposition


/*--------------------------------------------------------------------
	Get the element material (applicable to elements with a single, homogenous material)
 
	return: The element material (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
std::optional<Material> Roof::getMaterial() const {
#ifdef ARCHICAD
#ifdef ServerMainVers_2700
	return Material{Guid{Guid::fromInt(m_data->root.shellBase.buildingMaterial.GenerateHashValue())}};
#else
	return Material{Guid{Guid::fromInt(m_data->root.shellBase.buildingMaterial)}};
#endif
#endif
} //Roof::getMaterial


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure Roof::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().roof.topSurface);
	BIMMemory::setMask(&surveyor.elementMask().roof.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().roof.topSurface, surveyor.quantity().roof.volume};
#endif
} //Roof::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Roof::getHead() const {
	return m_data->root.head;
} //Roof::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Roof::getHead() {
	return m_data->root.head;
} //Roof::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Roof::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //Roof::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Roof::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //Roof::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Roof::setDefault() {
	m_data.reset();
} //Roof::setDefault
