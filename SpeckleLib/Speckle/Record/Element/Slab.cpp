#include "Speckle/Record/Element/Slab.h"

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

	class Slab::Data {
	public:
		friend class Slab;

#ifdef ARCHICAD
		Data(const API_SlabType& seg) : root{seg} {}

	private:
		API_SlabType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Slab::Slab() {
} //Slab::Slab


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Slab::Slab(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.slab);
} //Slab::Slab
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Slab::Slab(const Slab& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Slab::Slab


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Slab::Slab(Slab&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //Slab::Slab


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Slab::~Slab() {}


/*--------------------------------------------------------------------
	Get the composition of materials in the element
 
	return: The material composition (element with ordered material composition should override)
  --------------------------------------------------------------------*/
ModelElement::Composition Slab::getComposition() const {
#ifdef ARCHICAD
	return convert(m_data->root.modelElemStructureType);
#endif
} //Slab::getComposition


/*--------------------------------------------------------------------
	Get the element material (applicable to elements with a single, homogenous material)
 
	return: The element material (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
std::optional<Material> Slab::getMaterial() const {
#ifdef ARCHICAD
	return Material{Guid{Guid::fromInt(m_data->root.buildingMaterial.GenerateHashValue())}};
#endif
} //Slab::getMaterial


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure Slab::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().slab.topSurface);
	BIMMemory::setMask(&surveyor.elementMask().slab.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().slab.topSurface, surveyor.quantity().slab.volume};
#endif
} //Slab::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Slab::getHead() const {
	return m_data->root.head;
} //Slab::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Slab::getHead() {
	return m_data->root.head;
} //Slab::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Slab::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //Slab::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Slab::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //Slab::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Slab::setDefault() {
	m_data.reset();
} //Slab::setDefault
