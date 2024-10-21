#include "Speckle/Record/Element/Beam.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class Beam::Data {
	public:
		friend class Beam;

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{ std::make_unique<API_BeamType>(elem.beam) } {}
		Data(const Data& source) : root{ std::make_unique<API_BeamType>(*source.root) } {}
#endif

	private:
		std::unique_ptr<API_BeamType> root;
	};

}

namespace {

	///Serialisation fields
	enum FieldIndex {
		segmentID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"segments"},
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Beam::Beam() {
} //Beam::Beam


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Beam::Beam(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //Beam::Beam
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Beam::Beam(const Beam& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Beam::Beam


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Beam::~Beam() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Beam::getHead() const {
	return m_data->root->head;
} //Beam::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Beam::getHead() {
	return m_data->root->head;
} //Beam::getHead


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Beam::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
		//Establish the memo filter for this element
	if (!SegmentedBeam::isMemoLoaded())
		filter |= SegmentedBeam::getPartFilter();
	Element::loadMemo(filter, memo);
		//Receive the memo data into the element (when available)
	if (memo) {
		if (filter & SegmentedBeam::getPartFilter())
			SegmentedBeam::receive(*memo);
	}
	SegmentedBeam::setMemoLoaded(true); //Always mark the data as loaded to prevent repeated attempts on error
} //Beam::loadMemo
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Beam::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[segmentID], segmentID, getSegmentCount(), std::nullopt },	//TODO: implement other fields
		},
	}.withType(&typeid(Beam)));
	return base::fillInventory(inventory);
} //Beam::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Beam::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Beam))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
	case segmentID:
		if (auto segment = getSegment(item.available); segment != nullptr) {
			return Cargo::Unique{new PackageWrap{*segment}};
		} else
			return nullptr;
	default:
		return nullptr;	//Requested an unknown index
	}
} //Beam::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Beam::setDefault() {
	base::setDefault();
	m_data.reset();
} //Beam::setDefault
