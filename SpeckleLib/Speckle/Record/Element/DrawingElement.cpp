#include "Speckle/Record/Element/DrawingElement.h"

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

	class DrawingElement::Data {
	public:
		friend class DrawingElement;

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
DrawingElement::DrawingElement() {
} //DrawingElement::DrawingElement


/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The attribute table ID (attribute type)
  --------------------------------------------------------------------*/
DrawingElement::DrawingElement(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData);
} //DrawingElement::DrawingElement


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
DrawingElement::DrawingElement(const DrawingElement& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //DrawingElement::DrawingElement


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
DrawingElement::~DrawingElement() {}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& DrawingElement::getHead() const {
	return m_data->root->header;
} //DrawingElement::getHead

/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& DrawingElement::getHead() {
	return m_data->root->header;
} //DrawingElement::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool DrawingElement::fillInventory(Inventory& inventory) const {
	return base::fillInventory(inventory);	//Not implemented yet
} //DrawingElement::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique DrawingElement::getCargo(const Inventory::Item& item) const {
	return base::getCargo(item);	//Not implemented yet
} //DrawingElement::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void DrawingElement::setDefault() {
	base::setDefault();
	m_data.reset();
} //DrawingElement::setDefault
