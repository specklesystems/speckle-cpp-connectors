#include "Speckle/Database/Storage/ArchicadDBase/ArchicadDBaseCore.h"

#ifdef ARCHICAD

#include "Active/Utility/Defer.h"
#include "Active/Utility/Memory.h"
#include "Active/Utility/String.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Event/Type/DocStoreMergeEvent.h"
#include "Speckle/Event/Type/ProjectEvent.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <ACAPinc.h>
#include <BM.hpp>

using namespace active::event;
using namespace active::setting;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::event;
using namespace speckle::utility;

using enum ArchicadDBaseCore::Status;

namespace {
	
	/*--------------------------------------------------------------------
		Convert an Archicad API error code to a ArchicadDBaseCore status
	 
		acErrorCode: The API error code
	 
		return: An equivalent status code
	 --------------------------------------------------------------------*/
	ArchicadDBaseCore::Status convertArchicadError(long acErrorCode) {
		using enum ArchicadDBaseCore::Status;
		switch (acErrorCode) {
			case NoError:
				return nominal;
			case APIERR_BADPARS:
				return badParameter;
			case APIERR_BADID:
				return badID;
			default:
				break;
		}
		return error;
	} //convertArchicadError


		///Category for ArchicadElementDBase processing errors
	class ArchicadDBaseCategory : public std::error_category {
	public:
			///Category name
		const char* name() const noexcept override {
			return "speckle::database::archicad::category";
		}
		/*!
			Get a message for a specified error code
			@param errorCode A ArchicadElementDBase processing code
			@return The error message for the specified code
		*/
		std::string message(int errorCode) const override {
				//TODO: These error messages are ok for developers - but can we help users more?
			switch (static_cast<ArchicadDBaseCore::Status>(errorCode)) {
				case nominal:
					return "";
				case badParameter:
					return "An internal function has been incorrectly used";
				case badID:
					return "Internal data has been requested using an invalid identity";
				case error:
					return "A non-specific error occurred";
				default:
					return "An unknown error occurred";
			}
		}
	};

		///ArchicadElementDBase processing category error instance
	static ArchicadDBaseCategory instance;
	
}

/*--------------------------------------------------------------------
	Make an error code for ArchicadElementDBase processing
 
	return: An STL error code
 --------------------------------------------------------------------*/
std::error_code ArchicadDBaseCore::makeError(ArchicadDBaseCore::Status code) {
	return std::error_code(static_cast<int>(code), instance);
} //ArchicadDBaseCore::makeError

#endif
