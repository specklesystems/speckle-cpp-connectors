#ifndef SPECKLE_SERIALISE_FINISH_COLLECTOR
#define SPECKLE_SERIALISE_FINISH_COLLECTOR

#include "Active/Serialise/Management/Manager.h"

namespace speckle::record::attribute {
	class Finish;
}

namespace speckle::serialise {
	
	/*!
	 Collector for object finishes
	 
	 Used as a serialisation manager to collect finishes from serialised objects
	 */
	class FinishCollector : public virtual active::serialise::Manager {
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
		virtual bool addFinishProxy(const speckle::database::BIMIndex& materialIndex, const speckle::database::BIMRecordID& objectID) = 0;
		/*!
		 Add a ModelerAPI material to the collection (NB: These are not persistent so need to be captured by this method)
		 @param finish A finish
		 @param objectID The object the material is applied to
		 @return True if the material proxy was added (false typically means the record already exists)
		 */
		virtual bool addFinishProxy(const record::attribute::Finish& finish, const speckle::database::BIMRecordID& objectID) = 0;
	};
	
}

#endif	//SPECKLE_SERIALISE_FINISH_COLLECTOR
