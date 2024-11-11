#include "Speckle/Record/Element/MeshElem.h"

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

	class Mesh::Data {
	public:
		friend class Mesh;

#ifdef ARCHICAD
		Data(const API_MeshType& seg) : root{seg} {}

	private:
		API_MeshType root;
#endif
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Mesh::Mesh() {
} //Mesh::Mesh


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor

	elemData: Archicad element data
	tableID: The element table ID (AC database, e.g. floor plan, 3D)
  --------------------------------------------------------------------*/
Mesh::Mesh(const API_Element& elemData, const speckle::utility::Guid& tableID) : base{ elemData.header.guid, tableID } {
	m_data = std::make_unique<Data>(elemData.mesh);
} //Mesh::Mesh
#endif


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Mesh::Mesh(const Mesh& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*source.m_data) : nullptr;
} //Mesh::Mesh


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Mesh::Mesh(Mesh&& source) noexcept : base{source} {
	m_data = std::move(source.m_data);
} //Mesh::Mesh


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Mesh::~Mesh() {}


/*--------------------------------------------------------------------
	Get the element material (applicable to elements with a single, homogenous material)
 
	return: The element material (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
std::optional<Material> Mesh::getMaterial() const {
#ifdef ARCHICAD
	return Material{Guid{Guid::fromInt(m_data->root.buildingMaterial.GenerateHashValue())}};
#endif
} //Mesh::getMaterial


/*--------------------------------------------------------------------
	Get a spatial measurement from this element (area/volume, as used for material quantities)
 
	return: The element measurement (empty if unavailable)
  --------------------------------------------------------------------*/
ModelElement::SpatialMeasure Mesh::getSpatialMeasure() const {
#ifdef ARCHICAD
	Surveyor surveyor;
	BIMMemory::setMask(&surveyor.elementMask().mesh.topSurface);
	BIMMemory::setMask(&surveyor.elementMask().mesh.volume);
	surveyor.measure(getBIMLink());
	return SpatialMeasure{surveyor.quantity().mesh.topSurface, surveyor.quantity().mesh.volume};
#endif
} //Mesh::getSpatialMeasure


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get the (immutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
const API_Elem_Head& Mesh::getHead() const {
	return m_data->root.head;
} //Mesh::getHead


/*--------------------------------------------------------------------
	Get the (mutable) API element header data

	return: The element header data (only use this data for low-level operations - for normal code, call getters/setters)
  --------------------------------------------------------------------*/
API_Elem_Head& Mesh::getHead() {
	return m_data->root.head;
} //Mesh::getHead
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Mesh::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
		//TODO: Implement other fields as required
	return base::fillInventory(inventory);
} //Mesh::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Mesh::getCargo(const Inventory::Item& item) const {
		//TODO: Implement other fields as required
	return base::getCargo(item);
} //Mesh::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Mesh::setDefault() {
	m_data.reset();
} //Mesh::setDefault
