#include "Speckle/Record/Element/Wall.h"

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

	class Wall::Data {
	public:
		friend class Wall;

#ifdef ARCHICAD
		Data(const API_WallType& seg) : root{seg} {}

	private:
		API_WallType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Wall::Wall() {
} //Wall::Wall


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Wall::Wall(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.wall);
} //Wall::Wall
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Wall::Wall(const Wall& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Wall::Wall


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Wall::Wall(Wall&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //Wall::Wall


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Wall::~Wall() {}


/*--------------------------------------------------------------------
	Get the composition of materials in the element
 
	return: The material composition (element with ordered material composition should override)
  --------------------------------------------------------------------*/
ModelElement::Composition Wall::getComposition() const {
#ifdef ARCHICAD
	return convert(m_data->root.modelElemStructureType);
#endif
} //Wall::getComposition


/*--------------------------------------------------------------------
	Get the element material (applicable to elements with a single, homogenous material)
 
	return: The element material (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
std::optional<Material> Wall::getMaterial() const {
#ifdef ARCHICAD
	return Material{Guid{Guid::fromInt(m_data->root.buildingMaterial.GenerateHashValue())}};
#endif
} //Wall::getMaterial


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure Wall::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().wall.surface1);
	BIMMemory::setMask(&surveyor.elementMask().wall.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().wall.surface1, surveyor.quantity().wall.volume};
#endif
} //Wall::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Wall::getHead() const {
	return m_data->root.head;
} //Wall::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Wall::getHead() {
	return m_data->root.head;
} //Wall::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Wall::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //Wall::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Wall::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //Wall::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Wall::setDefault() {
	m_data.reset();
} //Wall::setDefault
