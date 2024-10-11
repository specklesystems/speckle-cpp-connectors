#ifndef SPECKLE_SERIALISE_FINISH_COLLECTOR
#define SPECKLE_SERIALISE_FINISH_COLLECTOR

#include "Active/Serialise/Management/Manager.h"

#ifdef ARCHICAD
namespace ModelerAPI {
	class Material;
}
#endif

namespace speckle::serialise {
	
	/*!
	 Collector for object finishes
	 
	 Used as a serialisation manager to collect finishes from serialised objects
	 */
	class FinishCollector : public active::serialise::Manager {
	public:
		/*!
		 Destructor
		 */
		virtual ~FinishCollector() {}

		// MARK: - Functions (mutating)

		/*!
		 Add a material proxy record to the collection
		 @param materialIndex The index of the material to add
		 @param objectID The object the material is applied to
		 @return True if the material proxy was added (false typically means the record already exists)
		 */
		virtual bool addMaterialProxy(const speckle::database::BIMIndex& materialIndex, const speckle::database::BIMRecordID& objectID) = 0;
#ifdef ARCHICAD
		/*!
		 Add a ModelerAPI material to the collection (NB: These are not persistent so need to be captured by this method)
		 @param material A material
		 @param objectID The object the material is applied to
		 @return True if the material proxy was added (false typically means the record already exists)
		 */
		virtual bool addMaterialProxy(const ModelerAPI::Material& material, const speckle::database::BIMRecordID& objectID) = 0;
#endif
	};
	
}

#endif	//SPECKLE_SERIALISE_FINISH_COLLECTOR
