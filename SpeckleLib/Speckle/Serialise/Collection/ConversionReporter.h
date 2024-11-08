#ifndef SPECKLE_SERIALISE_CONVERSION_REPORTER
#define SPECKLE_SERIALISE_CONVERSION_REPORTER

#include "Active/Serialise/Management/Manager.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::interfac {
	class Progress;
}

namespace speckle::serialise {
	
	/*!
	 Manager for reporting on the progress of a send conversion/serialisation process
	 */
	class ConversionReporter : public virtual active::serialise::Manager {
	public:
		
			///Data collected from the conversion of a record
		struct Data {
				///Record conversion status
			enum class Status {
				success,
				failure,
			};
			
				///The conversion status
			Status status;
				///The name of the record type
			utility::String typeName;
				///The name of the record type
			utility::String speckleType;
				///Optional message from the conversion (particularly relevant when an error occurs)
			utility::String message;
		};
		
			///Reporter log pairing a record ID with the outcome of its conversion
		using Log = std::unordered_map<database::BIMRecordID, Data>;
		
		// MARK: Constructors
		
		/*!
		 Destructor
		 */
		virtual ~ConversionReporter();

		// MARK: - Functions (const)

		/*!
		 Get the conversion report model card ID
		 @return The model card ID
		 */
		const database::RecordID& getModelCardID() const { return m_modelCardID; }
		/*!
		 Get the conversion log
		 @return The conversion log
		 */
		const Log& getLog() const { return m_log; }

		// MARK: - Functions (mutating)

		/*!
		 Set the conversion report model card ID
		 @param cardID The model card ID
		 */
		void setModelCardID(const database::RecordID& cardID) { m_modelCardID = cardID; }
		/*!
		 Increment the number of projected records to be logged
		 */
		void incrementProjectedRecords() { ++m_projectedRecordCount; }
		/*!
		 Increment the number of skipped records (and add to the projected record count)
		 */
		void incrementSkippedRecords() {
			++m_projectedRecordCount;
			incrementProjectedRecords();
		}
		/*!
		 Log the conversion/serialisation of a record
		 @param recordID The ID of the serialised record
		 @param data The record serialisation report data
		 @param withUIFeedback True if the UI progress feedback should be updated
		 @return True if the serialisation should continue
		 */
		bool logRecord(const speckle::database::BIMRecordID& recordID, const Data& data = {Data::Status::success}, bool withUIFeedback = true);
		
	private:
			///The reporter log
		Log m_log;
			///The ID of the conversion model card
		database::RecordID m_modelCardID;
			///The conversion progress UI display
		std::shared_ptr<interfac::Progress> m_progress;
			///Projected number of records to be logged (used to calculate UI progress components)
		size_t m_projectedRecordCount = 0;
			///Number of records skipped (before processing starts - the UI progress feedback will reflect their completion)
		size_t m_skippedRecordCount = 0;
	};
	
}

#endif	//SPECKLE_SERIALISE_CONVERSION_REPORTER
