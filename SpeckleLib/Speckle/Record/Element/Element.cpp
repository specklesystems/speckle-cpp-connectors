#include "Speckle/Record/Element/Element.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/Guid.h"

using namespace active::serialise;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

#include <array>
#include <memory>

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Element::Element() {
} //Element::Element


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
Element::Element(const Guid& ID, const Guid& tableID, std::optional<active::measure::LengthType> unit) : base{ID, tableID, unit} {
} //Element::Element


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Element::Element(const Element& source) : base{ source } {
} //Element::Element


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Element::Element(Element&& source) : base{source} {
} //Element::Element


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Element::~Element() {}


/*--------------------------------------------------------------------
	Get the element storey

	return: The element storey (nullopt if the element isn't linked to a storey)
  --------------------------------------------------------------------*/
Storey::Option Element::getStorey() const {
#ifdef ARCHICAD
	return Storey{ getHead().floorInd };
#endif
} //Element::getStorey


/*--------------------------------------------------------------------
	Get the element type name, e.g. "Wall", "Roof" etc

	return: The type name
  --------------------------------------------------------------------*/
String Element::getTypeName() const {
#ifdef ARCHICAD
	GS::UniString typeName;
	if (auto err = ACAPI_Element_GetElemTypeName(getHead().type, typeName); err != NoError)
		return addon()->getLocalString(titleStringLib, unknownElementTypeID);
	return typeName;
#endif
} //Element::getTypeName


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Element::fillInventory(Inventory& inventory) const {
	return base::fillInventory(inventory);
} //Element::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Element::getCargo(const Inventory::Item& item) const {
	return base::getCargo(item);
} //Element::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Element::setDefault() {
	base::setDefault();
} //Element::setDefault


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Element::loadMemo(Part::filter_bits filter, std::unique_ptr<Memo>& memo) const {
		//If the memo data isn't loaded, fetch it now
	if (!memo) {
		auto project = addon()->getActiveProject().lock();
		if (!project)
			return;
		if (auto loaded = project->getElementDatabase()->getMemo(getBIMID(), filter); loaded)
			memo.reset(loaded.release());
	}
} //Element::loadMemo
