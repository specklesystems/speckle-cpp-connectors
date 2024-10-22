#include "Speckle/Record/Element/GenericDrawingElement.h"

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

	class GenericDrawingElement::Data {
	public:
		friend class GenericDrawingElement;

#ifdef ARCHICAD
		Data(const API_Element& elem) : root{ std::make_unique<API_Element>(elem) } {}
		Data(const Data& source) : root{ std::make_unique<API_Element>(*source.root) } {}
#endif

	private:
		std::unique_ptr<API_Element> root;
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GenericDrawingElement::GenericDrawingElement() {
} //GenericDrawingElement::GenericDrawingElement


/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The attribute table ID (attribute type)
  --------------------------------------------------------------------*/
GenericDrawingElement::GenericDrawingElement(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //GenericDrawingElement::GenericDrawingElement


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
GenericDrawingElement::GenericDrawingElement(const GenericDrawingElement& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //GenericDrawingElement::GenericDrawingElement


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
GenericDrawingElement::~GenericDrawingElement() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& GenericDrawingElement::getHead() const {
	return m_data->root->header;
} //GenericDrawingElement::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& GenericDrawingElement::getHead() {
	return m_data->root->header;
} //GenericDrawingElement::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool GenericDrawingElement::fillInventory(Inventory& inventory) const {
	return base::fillInventory(inventory);	//Not implemented yet
} //GenericDrawingElement::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique GenericDrawingElement::getCargo(const Inventory::Item& item) const {
	return base::getCargo(item);	//Not implemented yet
} //GenericDrawingElement::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void GenericDrawingElement::setDefault() {
	base::setDefault();
	m_data.reset();
} //GenericDrawingElement::setDefault