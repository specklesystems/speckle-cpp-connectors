#include "Speckle/Record/Element/ColumnSegment.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/Guid.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace active::serialise;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class ColumnSegment::Data {
	public:
		friend class ColumnSegment;

#ifdef ARCHICAD
		Data(const API_ColumnSegmentType& seg) : root{ std::make_unique<API_ColumnSegmentType>(seg) } {}
		Data(const Data& source) : root{ std::make_unique<API_ColumnSegmentType>(*source.root) } {}

	private:
		std::unique_ptr<API_ColumnSegmentType> root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ColumnSegment::ColumnSegment() {
} //ColumnSegment::ColumnSegment


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
ColumnSegment::ColumnSegment(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.columnSegment);
} //ColumnSegment::ColumnSegment


/*--------------------------------------------------------------------
	Constructor
 
	segment: The segment element data
	tableID: The parent table ID
	cutOrigin: Cut at the segment origin
	cutEnd: Cut at the segment end
	scheme: The segment scheme
	profile: The segment profile (nullptr = none)
  --------------------------------------------------------------------*/
ColumnSegment::ColumnSegment(const API_ColumnSegmentType& segment, const speckle::utility::Guid& tableID, const API_AssemblySegmentCutData& cutOrigin,
							 const API_AssemblySegmentCutData& cutEnd, const API_AssemblySegmentSchemeData& scheme,
							 const API_AssemblySegmentProfileData* profile) :
		base{segment.head.guid, tableID}, assembly::Segment{cutOrigin, cutEnd, scheme, profile} {
	m_data = std::make_unique<Data>(segment);
} //ColumnSegment::ColumnSegment
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
ColumnSegment::ColumnSegment(const ColumnSegment& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //ColumnSegment::ColumnSegment


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ColumnSegment::~ColumnSegment() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& ColumnSegment::getHead() const {
	return m_data->root->head;
} //ColumnSegment::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& ColumnSegment::getHead() {
	return m_data->root->head;
} //ColumnSegment::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ColumnSegment::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //ColumnSegment::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ColumnSegment::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //ColumnSegment::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ColumnSegment::setDefault() {
	m_data.reset();
} //ColumnSegment::setDefault
