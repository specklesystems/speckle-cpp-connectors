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
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"vertices"},
		Identity{"faces"},
		Identity{"colors"},
	};

}

/*--------------------------------------------------------------------
	Append a single face to the Mesh given by the vertices
  --------------------------------------------------------------------*/
void Mesh::appendFace(const std::vector<double>& vertices) {
	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	int lastVertexIndex = m_faces.back();
	int faceSize = vertices.size() / 3;
	m_faces.push_back(faceSize);
	for (int i = 0; i < faceSize; i++)
	{
		m_faces.push_back(++lastVertexIndex);
	}
}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Mesh::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[vertexID], vertexID, element },
			{ fieldID[faceID], faceID, element },
			{ fieldID[colorID], colorID, element },
		},
	}.withType(&typeid(Mesh)));
	return base::fillInventory(inventory);
} //Mesh::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Mesh::getCargo(const Inventory::Item& item) const {
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
} //Mesh::getCargo


/*--------------------------------------------------------------------
	Use a manager in (de)serialisation processes
 
	management: The management to use
  --------------------------------------------------------------------*/
void Mesh::useManagement(Management* management) const {
		//NB: This object only exists to populate the finish collection - it doesn't carry any serialisable content
	if (management != nullptr) {
		if (auto collector = management->get<FinishCollector>(); collector != nullptr)
			collector->addMaterialProxy(m_material, getBIMID());
	}
} //Mesh::useManagement
