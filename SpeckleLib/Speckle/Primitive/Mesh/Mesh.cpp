#include "Speckle/Primitive/Mesh/Mesh.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Active/Serialise/Inventory/Identity.h"
#include "Speckle/Serialise/Collection/FinishProxy.h"

#include <array>

using namespace active::serialise;
using namespace speckle::primitive;
using namespace speckle::serialise;

namespace {

		///Serialisation fields
	enum FieldIndex {
		vertexID,
		faceID,
		colorID,
		pointID,
		closedID,
		lengthID,
		areaID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"vertices"},
		Identity{"faces"},
		Identity{"colors"},
		Identity{"value"},
		Identity{"closed"},
		Identity{"length"},
		Identity{"area"},
	};

}

/*--------------------------------------------------------------------
	Append a single face to the Mesh given by the vertices
  --------------------------------------------------------------------*/
void Mesh::appendFace(const std::vector<double>& vertices) {
	if (vertices.empty())
		return;

	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	int lastVertexIndex = m_faces.empty() ? -1 : m_faces.back();
	int faceSize = static_cast<int>(vertices.size() / 3);
	m_faces.push_back(faceSize);
	for (size_t i = 0; i < faceSize; i++)
		m_faces.push_back(++lastVertexIndex);
}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Mesh::fillInventory(Inventory& inventory) const {
	if (isPolyline)
		return fillInventoryPolyline(inventory);
	else
		return fillInventoryMesh(inventory);
} //Mesh::fillInventory

bool Mesh::fillInventoryMesh(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[vertexID], vertexID, element },
			{ fieldID[faceID], faceID, element },
			{ fieldID[colorID], colorID, element },
		},
		}.withType(&typeid(Mesh)));
	return base::fillInventory(inventory);
}

bool Mesh::fillInventoryPolyline(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[pointID], pointID, element },
			{ fieldID[closedID], closedID, element },
			{ fieldID[lengthID], lengthID, element },
			{ fieldID[areaID], areaID, element },
		},
		}.withType(&typeid(Mesh)));
	return base::fillInventory(inventory);
}


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Mesh::getCargo(const Inventory::Item& item) const {
	if (isPolyline)
		return getCargoPolyline(item);
	else
		return getCargoMesh(item);
	
} //Mesh::getCargo

Cargo::Unique Mesh::getCargoMesh(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Mesh))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
	case vertexID:
		return std::make_unique<ContainerWrap<std::vector<double>>>(m_vertices);
	case faceID:
		return std::make_unique<ContainerWrap<std::vector<int>>>(m_faces);
	case colorID:
		return std::make_unique<ContainerWrap<std::vector<int>>>(m_colors);
	default:
		return nullptr;	//Requested an unknown index
	}
}

Cargo::Unique Mesh::getCargoPolyline(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Mesh))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
	case pointID:
		return std::make_unique<ContainerWrap<std::vector<double>>>(m_points);
	case closedID:
		return std::make_unique<ValueWrap<bool>>(isClosed);
	case lengthID:
		return std::make_unique<ValueWrap<double>>(length);
	case areaID:
		return std::make_unique<ValueWrap<double>>(area);
	default:
		return nullptr;	//Requested an unknown index
	}
}


/*--------------------------------------------------------------------
	Use a manager in (de)serialisation processes
 
	management: The management to use
  --------------------------------------------------------------------*/
void Mesh::useManagement(Management* management) const {
		//NB: This object only exists to populate the finish collection - it doesn't carry any serialisable content
	if (management != nullptr) {
		if (auto collector = management->get<FinishCollector>(); collector != nullptr)
			collector->addMaterialProxy(m_finish, getBIMID());
	}
} //Mesh::useManagement
