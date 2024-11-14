#include "Speckle/Record/Element/StairStructure.h"

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

	class StairStructure::Data {
	public:
		friend class StairStructure;

#ifdef ARCHICAD
		Data(const API_StairStructureType& seg) : root{seg} {}

	private:
		API_StairStructureType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
StairStructure::StairStructure() {
} //StairStructure::StairStructure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
StairStructure::StairStructure(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.stairStructure);
} //StairStructure::StairStructure


/*--------------------------------------------------------------------
	Constructor
 
	structure: The structure element data
	tableID: The parent table ID
  --------------------------------------------------------------------*/
StairStructure::StairStructure(const API_StairStructureType& structure, const speckle::utility::Guid& tableID) : base{structure.head.guid, tableID} {
	 m_data = std::make_unique<Data>(structure);
} //StairStructure::StairStructure
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
StairStructure::StairStructure(const StairStructure& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //StairStructure::StairStructure


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
StairStructure::StairStructure(StairStructure&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //StairStructure::StairStructure


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
StairStructure::~StairStructure() {}


/*--------------------------------------------------------------------
	Get the element material (applicable to elements with a single, homogenous material)
 
	return: The element material (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
std::optional<Material> StairStructure::getMaterial() const {
#ifdef ARCHICAD
		//Only a monolithic stair structure component has a material, e.g. precast concrete
	if (m_data->root.structType != APIST_Monolith)
		return std::nullopt;
#ifdef ServerMainVers_2700
	return Material{Guid{Guid::fromInt(m_data->root.data.monolith.buildingMaterial.GenerateHashValue())}};
#else
	return Material{Guid{Guid::fromInt(m_data->root.data.monolith.buildingMaterial)}};
#endif //ServerMainVers_2700
#endif //ARCHICAD
} //StairStructure::getMaterial


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure StairStructure::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().stairStructure.thickness);
	BIMMemory::setMask(&surveyor.elementMask().stairStructure.volume);
	surveyor.measure(getBIMLink());
		//NB: Surface area is estimated by dividing the structure volume by the thickness - may need to be reviewed in different contexts
	return SpatialMeasure{surveyor.quantity().stairStructure.volume / surveyor.quantity().stairStructure.thickness, surveyor.quantity().stairStructure.volume};
#endif
} //StairStructure::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& StairStructure::getHead() const {
	return m_data->root.head;
} //StairStructure::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& StairStructure::getHead() {
	return m_data->root.head;
} //StairStructure::getHead
#endif //ARCHICAD


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool StairStructure::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //StairStructure::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique StairStructure::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //StairStructure::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void StairStructure::setDefault() {
	m_data.reset();
} //StairStructure::setDefault
