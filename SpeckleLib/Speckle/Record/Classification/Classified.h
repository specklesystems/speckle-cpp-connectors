#ifndef SPECKLE_RECORD_ELEMENT_CLASSIFIED
#define SPECKLE_RECORD_ELEMENT_CLASSIFIED

#include "Speckle/Database/Identity/BIMRecordID.h"

namespace speckle::record::classify {
	
	/*!
	 Interface for records with attached classifications
	 */
	class Classified {
	public:

		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<Classified>;
			///Shared pointer
		using Shared = std::shared_ptr<Classified>;
			///Optional
		using Option = std::optional<Classified>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Classified() {}
		/*!
		 Destructor
		 */
		virtual ~Classified() {}

		// MARK: - Functions (const)

		/*!
		 Get the IDs of all classifications attached to the record
		 @return The list of classification IDs
		 */
		database::BIMRecordIDList getClassificationIDs() const;
		
		// MARK: - Functions (mutating)

	};

}

#endif	//SPECKLE_RECORD_ELEMENT_CLASSIFIED
