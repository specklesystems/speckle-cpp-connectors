#ifndef SPECKLE_SERIALISE_FINISH_PROXY
#define SPECKLE_SERIALISE_FINISH_PROXY

#include "Active/Serialise/Management/Management.h"

#include "Speckle/Serialise/Collection/FinishCollector.h"

#ifdef ARCHICAD
namespace ModelerAPI {
	class Material;
}
#endif

namespace speckle::serialise {
	
	/*!
	 Proxy cargo for tranporting a mesh finish
	 
	 Mesh finishes are currently not serialised with the mesh - instead, they are gathered into a 'material proxy' container that points back to
	 the target meshes. This proxy allows the mesh to inform the finishes collector (acting as a serialisation manager) to link the mesh with
	 its finish so the manager can build the 'material proxies' at the end of the project collection.
	 */
	class FinishProxy : public active::serialise::Package {
	public:
		FinishProxy(const database::BIMRecordID& meshID, const ModelerAPI::Material& material) : m_objectID{meshID}, m_material{material} {}
		
		/*!
		 Fill an inventory with the package items
		 @param inventory The inventory to receive the package items
		 @return: True if the package has added items to the inventory
		 */
		bool fillInventory(active::serialise::Inventory& inventory) const {
				//NB: This object only exists to populate the finish collection - it doesn't carry any serialisable content
			if (management() != nullptr) {
				if (auto collector = management()->get<FinishCollector>(); collector != nullptr)
					collector->addMaterialProxy(m_material, m_objectID);
			}
			return true;
		} //DocumentInfo::fillInventory
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure)
		 */
		active::serialise::Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const { return nullptr; }	//Empty placeholder

	private:
		const database::BIMRecordID& m_objectID;
		const ModelerAPI::Material& m_material;
	};
	
}

#endif	//SPECKLE_SERIALISE_FINISH_PROXY
