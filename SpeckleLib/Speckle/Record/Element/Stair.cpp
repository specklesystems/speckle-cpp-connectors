#include "Speckle/Record/Element/Stair.h"

#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Speckle/Record/Element/StairRiser.h"
#include "Speckle/Record/Element/StairStructure.h"
#include "Speckle/Record/Element/StairTread.h"

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class Stair::Data {
	public:
		friend class Stair;

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{elem.stair} {}
#endif

	private:
		API_StairType root;
	};

}

namespace {

	///Serialisation fields
	enum FieldIndex {
		riserID,
		structStairID,
		treadID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"risers"},
		Identity{"structures"},
		Identity{"treads"},
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Stair::Stair() {
} //Stair::Stair


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Stair::Stair(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //Stair::Stair
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Stair::Stair(const Stair& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Stair::Stair


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Stair::~Stair() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Stair::getHead() const {
	return m_data->root.head;
} //Stair::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Stair::getHead() {
	return m_data->root.head;
} //Stair::getHead


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Stair::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
		//Establish the memo filter for this element
	if (!Risers::isMemoLoaded())
		filter |= Risers::getPartFilter();
	if (!StructuredStair::isMemoLoaded())
		filter |= StructuredStair::getPartFilter();
	if (!Treads::isMemoLoaded())
		filter |= Treads::getPartFilter();
	ModelElement::loadMemo(filter, memo);
		//Receive the memo data into the element (when available)
	if (memo) {
		if (filter & Risers::getPartFilter())
			Risers::receive(*memo);
		if (filter & StructuredStair::getPartFilter())
			StructuredStair::receive(*memo);
		if (filter & Treads::getPartFilter())
			Treads::receive(*memo);
	}
	Risers::setMemoLoaded(true); //Always mark the data as loaded to prevent repeated attempts on error
	StructuredStair::setMemoLoaded(true); //Always mark the data as loaded to prevent repeated attempts on error
	Treads::setMemoLoaded(true); //Always mark the data as loaded to prevent repeated attempts on error
} //Stair::loadMemo
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Stair::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[riserID], riserID, getRiserCount(), std::nullopt },	//TODO: implement other fields
			{ fieldID[structStairID], structStairID, getStructureCount(), std::nullopt },	//TODO: implement other fields
			{ fieldID[treadID], treadID, getTreadCount(), std::nullopt },	//TODO: implement other fields
		},
	}.withType(&typeid(Stair)));
	return base::fillInventory(inventory);
} //Stair::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Stair::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Stair))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
	case riserID:
		if (auto riser = getRiser(item.available); riser != nullptr) {
			return Cargo::Unique{new PackageWrap{*riser}};
		} else
			return nullptr;
	case structStairID:
		if (auto structure = getStructure(item.available); structure != nullptr) {
			return Cargo::Unique{new PackageWrap{*structure}};
		} else
			return nullptr;
	case treadID:
		if (auto tread = getTread(item.available); tread != nullptr) {
			return Cargo::Unique{new PackageWrap{*tread}};
		} else
			return nullptr;
	default:
		return nullptr;	//Requested an unknown index
	}
} //Stair::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Stair::setDefault() {
	base::setDefault();
	m_data.reset();
} //Stair::setDefault
