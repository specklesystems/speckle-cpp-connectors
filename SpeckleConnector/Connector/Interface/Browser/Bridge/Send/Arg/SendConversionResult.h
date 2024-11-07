#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_CONVERSION_RESULT
#define CONNECTOR_INTERFACE_BRIDGE_SEND_CONVERSION_RESULT

#include "Connector/Interface/Browser/Bridge/Send/Arg/ConversionResult.h"
#include "Speckle/Serialise/Collection/ConversionReporter.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A result from converting a record to Speckle
	 */
	class SendConversionResult : public ConversionResult {
	public:
				
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param stat The send/conversion status of the target record
		 @param srcID The record application ID
		 @param srcType The record application type name
		 @param err An optional error report
		 */
		SendConversionResult(Status stat, speckle::utility::String srcID, speckle::utility::String srcType, SendError::Option err = std::nullopt) {
			status = stat;
			sourceID = srcID;
			sourceType = srcType;
			error = err;
		}
		/*!
		 Constructor
		 @param stat The send/conversion status of the target record
		 @param srcID The record application ID
		 @param srcType The record application type name
		 @param err An optional error report
		 */
		SendConversionResult(const speckle::database::BIMRecordID& srcID, const speckle::serialise::ConversionReporter::Data& data) {
			status = data.status == speckle::serialise::ConversionReporter::Data::Status::success ?
					ConversionResult::Status::success : ConversionResult::Status::error;
			sourceID = srcID.operator speckle::utility::String();
			sourceType = data.typeName;
			if (!data.message.empty())
				error = SendError{data.message};
		}
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_CONVERSION_RESULT
