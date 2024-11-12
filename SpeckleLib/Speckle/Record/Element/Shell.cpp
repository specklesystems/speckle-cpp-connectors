#include "Speckle/Record/Element/Shell.h"

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

	class Shell::Data {
	public:
		friend class Shell;

#ifdef ARCHICAD
		Data(const API_ShellType& seg) : root{seg} {}

	private:
		API_ShellType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Shell::Shell() {
} //Shell::Shell


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Shell::Shell(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.shell);
} //Shell::Shell
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Shell::Shell(const Shell& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Shell::Shell


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Shell::Shell(Shell&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //Shell::Shell


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Shell::~Shell() {}


/*--------------------------------------------------------------------
	Get the element material (applicable to elements with a single, homogenous material)
 
	return: The element material (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
std::optional<Material> Shell::getMaterial() const {
#ifdef ARCHICAD
#ifdef ServerMainVers_2600
	return Material{Guid{Guid::fromInt(m_data->root.shellBase.buildingMaterial.GenerateHashValue())}};
#else
	return Material{Guid{Guid::fromInt(m_data->root.shellBase.buildingMaterial)}};
#endif
#endif
} //Shell::getMaterial


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure Shell::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().shell.referenceSurface);
	BIMMemory::setMask(&surveyor.elementMask().shell.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().shell.referenceSurface, surveyor.quantity().shell.volume};
#endif
} //Shell::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Shell::getHead() const {
	return m_data->root.head;
} //Shell::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Shell::getHead() {
	return m_data->root.head;
} //Shell::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Shell::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //Shell::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Shell::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //Shell::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Shell::setDefault() {
	m_data.reset();
} //Shell::setDefault
