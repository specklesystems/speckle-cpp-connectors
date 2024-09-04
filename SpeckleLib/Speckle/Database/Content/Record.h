#ifndef SPECKLE_DATABASE_RECORD
#define SPECKLE_DATABASE_RECORD

#include "Active/Database/Content/Record.h"
#include "Speckle/Database/Identity/Link.h"

namespace speckle::database {

	/*!
	 Base class for a database record
	 */
	class Record : public active::database::Record<speckle::utility::String, speckle::utility::String, speckle::utility::String> {
	public:

		// MARK: - Types
		
		using base = active::database::Record<speckle::utility::String, speckle::utility::String, speckle::utility::String>;
			///Unique pointer
		using Unique = std::unique_ptr<Record>;
			///Shared pointer
		using Shared = std::shared_ptr<Record>;
			///Optional
		using Option = std::optional<Record>;

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param ID The record ID
		 */
		Record(speckle::utility::String ID) : base{ID} {}
		/*!
		 Destructor
		 */
		virtual ~Record() {}
		
		// MARK: - Functions (const)

		
		// MARK: - Functions (mutating)

		
		// MARK: - Serialisation
		
	};
	
}

#endif	//SPECKLE_DATABASE_RECORD
