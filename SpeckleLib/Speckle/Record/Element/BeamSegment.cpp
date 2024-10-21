#include "Speckle/Record/Element/BeamSegment.h"

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

	class BeamSegment::Data {
	public:
		friend class BeamSegment;

#ifdef ARCHICAD
		Data(const API_BeamSegmentType& seg) : root{ std::make_unique<API_BeamSegmentType>(seg) } {}
		Data(const Data& source) : root{ std::make_unique<API_BeamSegmentType>(*source.root) } {}

	private:
		std::unique_ptr<API_BeamSegmentType> root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
BeamSegment::BeamSegment() {
} //BeamSegment::BeamSegment


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
BeamSegment::BeamSegment(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.beamSegment);
} //BeamSegment::BeamSegment


/*--------------------------------------------------------------------
	Constructor
 
	segment: The segment element data
	tableID: The parent table ID
	cutOrigin: Cut at the segment origin
	cutEnd: Cut at the segment end
	scheme: The segment scheme
	profile: The segment profile (nullptr = none)
  --------------------------------------------------------------------*/
BeamSegment::BeamSegment(const API_BeamSegmentType& segment, const speckle::utility::Guid& tableID, const API_AssemblySegmentCutData& cutOrigin,
							 const API_AssemblySegmentCutData& cutEnd, const API_AssemblySegmentSchemeData& scheme,
							 const API_AssemblySegmentProfileData* profile) :
		base{segment.head.guid, tableID}, assembly::Segment{cutOrigin, cutEnd, scheme, profile} {
	m_data = std::make_unique<Data>(segment);
} //BeamSegment::BeamSegment
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
BeamSegment::BeamSegment(const BeamSegment& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //BeamSegment::BeamSegment


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
BeamSegment::BeamSegment(BeamSegment&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //BeamSegment::BeamSegment


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BeamSegment::~BeamSegment() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& BeamSegment::getHead() const {
	return m_data->root->head;
} //BeamSegment::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& BeamSegment::getHead() {
	return m_data->root->head;
} //BeamSegment::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool BeamSegment::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //BeamSegment::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique BeamSegment::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //BeamSegment::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void BeamSegment::setDefault() {
	m_data.reset();
} //BeamSegment::setDefault
