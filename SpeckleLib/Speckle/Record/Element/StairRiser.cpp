#include "Speckle/Record/Element/StairRiser.h"

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

	class StairRiser::Data {
	public:
		friend class StairRiser;

#ifdef ARCHICAD
		Data(const API_StairRiserType& seg) : root{seg} {}

	private:
		API_StairRiserType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
StairRiser::StairRiser() {
} //StairRiser::StairRiser


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
StairRiser::StairRiser(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.stairRiser);
} //StairRiser::StairRiser


/*--------------------------------------------------------------------
	Constructor
 
	riser: The riser element data
	tableID: The parent table ID
  --------------------------------------------------------------------*/
StairRiser::StairRiser(const API_StairRiserType& riser, const speckle::utility::Guid& tableID) : base{riser.head.guid, tableID} {
	 m_data = std::make_unique<Data>(riser);
} //StairRiser::StairRiser
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
StairRiser::StairRiser(const StairRiser& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //StairRiser::StairRiser


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
StairRiser::StairRiser(StairRiser&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //StairRiser::StairRiser


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
StairRiser::~StairRiser() {}


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure StairRiser::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().stairRiser.frontArea);
	BIMMemory::setMask(&surveyor.elementMask().stairRiser.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().stairRiser.frontArea, surveyor.quantity().stairRiser.volume};
#endif
} //StairRiser::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& StairRiser::getHead() const {
	return m_data->root.head;
} //StairRiser::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& StairRiser::getHead() {
	return m_data->root.head;
} //StairRiser::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool StairRiser::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //StairRiser::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique StairRiser::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //StairRiser::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void StairRiser::setDefault() {
	m_data.reset();
} //StairRiser::setDefault
