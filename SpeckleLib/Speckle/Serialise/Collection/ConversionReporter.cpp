#include "Speckle/Serialise/Collection/ConversionReporter.h"

#include "Speckle/Environment/Addon.h"
#include "Speckle/Interface/Component/Progress.h"
#include "Speckle/SpeckleResource.h"

using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::interfac;
using namespace speckle::serialise;

/*--------------------------------------------------------------------
	Destructor
 --------------------------------------------------------------------*/
ConversionReporter::~ConversionReporter() {}


/*--------------------------------------------------------------------
	Log the conversion/serialisation of a record
 
	recordID: The ID of the serialised record
	outcome: The record serialisation outcome
	withUIFeedback: True if the UI progress feedback should be updated
 
	return: True if the serialisation should continue
 --------------------------------------------------------------------*/
bool ConversionReporter::logRecord(const BIMRecordID& recordID, Outcome outcome, bool withUIFeedback) {
		//Initialise the progress UI component as required
	if (withUIFeedback && !m_progress) {
		if (m_progress = Progress::getInstance(addon()->getLocalString(titleStringLib, conversionProgressID)); m_progress) {
			m_progress->startStage(addon()->getLocalString(titleStringLib, conversionProgressID), m_projectedRecordCount);
				//If any records have already failed, mark them as complete in the UI progress display
			if (m_skippedRecordCount > 0)
				*m_progress += m_skippedRecordCount;
		}
	}
		//Log the conversion outcome for the record
	m_log[recordID] = outcome;
		//Update the progress UI component
	if (withUIFeedback && m_progress) {
		++(*m_progress);
		return !m_progress->isCancelled();
	}
	return true;
} //ConversionReporter::logRecord
