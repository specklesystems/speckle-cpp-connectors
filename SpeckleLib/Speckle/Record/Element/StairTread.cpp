#include "Speckle/Record/Element/StairTread.h"

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

	class StairTread::Data {
	public:
		friend class StairTread;

#ifdef ARCHICAD
		Data(const API_StairTreadType& seg) : root{seg} {}

	private:
		API_StairTreadType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
StairTread::StairTread() {
} //StairTread::StairTread


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
StairTread::StairTread(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.stairTread);
} //StairTread::StairTread


/*--------------------------------------------------------------------
	Constructor
 
	tread: The tread element data
	tableID: The parent table ID
  --------------------------------------------------------------------*/
StairTread::StairTread(const API_StairTreadType& tread, const speckle::utility::Guid& tableID) : base{tread.head.guid, tableID} {
	 m_data = std::make_unique<Data>(tread);
} //StairTread::StairTread
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
StairTread::StairTread(const StairTread& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //StairTread::StairTread


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
StairTread::StairTread(StairTread&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //StairTread::StairTread


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
StairTread::~StairTread() {}


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure StairTread::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().stairTread.area);
	BIMMemory::setMask(&surveyor.elementMask().stairTread.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().stairTread.area, surveyor.quantity().stairTread.volume};
#endif
} //StairTread::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& StairTread::getHead() const {
	return m_data->root.head;
} //StairTread::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& StairTread::getHead() {
	return m_data->root.head;
} //StairTread::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool StairTread::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //StairTread::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique StairTread::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //StairTread::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void StairTread::setDefault() {
	m_data.reset();
} //StairTread::setDefault
