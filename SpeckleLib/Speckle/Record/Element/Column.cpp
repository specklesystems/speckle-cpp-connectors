#include "Speckle/Record/Element/Column.h"

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

	class Column::Data {
	public:
		friend class Column;

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{ std::make_unique<API_ColumnType>(elem.column) } {}
		Data(const Data& source) : root{ std::make_unique<API_ColumnType>(*source.root) } {}
#endif

	private:
		std::unique_ptr<API_ColumnType> root;
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
Column::Column() {
} //Column::Column


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Column::Column(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //Column::Column
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Column::Column(const Column& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Column::Column


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Column::~Column() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Column::getHead() const {
	return m_data->root->head;
} //Column::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Column::getHead() {
	return m_data->root->head;
} //Column::getHead


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Column::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
		//Establish the memo filter for this element
	if (!SegmentedColumn::isMemoLoaded())
		filter |= SegmentedColumn::getPartFilter();
	ModelElement::loadMemo(filter, memo);
		//Receive the memo data into the element (when available)
	if (memo) {
		if (filter & SegmentedColumn::getPartFilter())
			SegmentedColumn::receive(*memo);
	}
	SegmentedColumn::setMemoLoaded(true); //Always mark the data as loaded to prevent repeated attempts on error
} //Column::loadMemo
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Column::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[segmentID], segmentID, getSegmentCount(), std::nullopt },	//TODO: implement other fields
		},
	}.withType(&typeid(Column)));
	return base::fillInventory(inventory);
} //Column::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Column::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Column))
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
} //Column::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Column::setDefault() {
	base::setDefault();
	m_data.reset();
} //Column::setDefault
