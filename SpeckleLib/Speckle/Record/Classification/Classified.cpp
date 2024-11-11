#include "Speckle/Record/Classification/Classified.h"

#include "Speckle/Database/Content/BIMRecord.h"

using namespace speckle::database;
using namespace speckle::record::classify;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Get the IDs of all classifications attached to the record
 
	return: The list of classification IDs (classification system ID paired with the leaf ID)
  --------------------------------------------------------------------*/
BIMRecordIDList Classified::getClassificationIDs() const {
		//TODO: Retrieve other classification data in future rather than just a list of IDs - this is a placeholder to support properties
	BIMRecordIDList result;
		//Establish that this is a BIM record (an can potentially have attached classifications)
	auto record = dynamic_cast<const BIMRecord*>(this);
	if (record != nullptr) {
#if ARCHICAD
		GS::Array<GS::Pair<API_Guid, API_Guid>> items;
			//Collect any classifications attached to the record
		if (ACAPI_Element_GetClassificationItems(record->getBIMID(), items) == NoError) {
			for (const auto& item : items)
				result.insert(item.second);
		}
#endif
	}
	return result;
} //Classified::getClassificationIDs
