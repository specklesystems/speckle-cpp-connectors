#ifndef SPECKLE_DATABASE_ARCHICAD_DBASE_CORE
#define SPECKLE_DATABASE_ARCHICAD_DBASE_CORE

#ifdef ARCHICAD

#include "Active/File/Path.h"
#include "Active/Setting/SettingList.h"
#include "Active/Database/Storage/DBaseSchema.h"
#include "Active/Utility/NameID.h"
#include "Speckle/Event/Subscriber/DocStoreSubscriber.h"
#include "Speckle/Event/Subscriber/ProjectSubscriber.h"

namespace speckle::database {
	
	using ArchicadDBaseSchema = active::database::DBaseSchema<>;

	/*!
	 Core functionality and definitions for a mechanism to store data in a BIM (3rd-party) document/database
	 
	 Currently implement for Archicad Add-On Objects
	 */
	class ArchicadDBaseCore  {
	public:

		// MARK: - Types
		
			///Status of of the ArchicadElementDBase database
		enum class Status {
			nominal,	///<No errors logged
			badParameter,	///<The data supplied to an SDK function call was invalid
			badID,	///<The ID for the stored data is invalid
			error,	///<An unidentified error occurred
		};
		
		// MARK: - Static functions
		
		/*!
		 Make an error code for ArchicadElementDBase processing
		 @return An STL error code
		 */
		static std::error_code makeError(ArchicadDBaseCore::Status code);

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param id The document storage identifier
		 @param schema The document storage schema
		 */
		ArchicadDBaseCore(const active::utility::NameID& id, ArchicadDBaseSchema&& schema) : m_id(id), m_schema{schema} {}
		ArchicadDBaseCore(const ArchicadDBaseCore&) = default;
		/*!
		 Destructor
		 */
		virtual ~ArchicadDBaseCore() {}
		
		// MARK: - Function (const)
		
		/*!
		 Get the database schema
		 @return The database schema
		 */
		const ArchicadDBaseSchema& getSchema() const { return m_schema; }
		/*!
		 Get the database id
		 @return The database id
		 */
		const active::utility::NameID& getID() const { return m_id; }
		
		// MARK: - Functions (mutating)
		
	protected:

		
	private:
			///The database schema
		ArchicadDBaseSchema m_schema;
			///The database ID (mutable to allow lazy loading when data is accessed)
		mutable active::utility::NameID m_id;
	};
	
}

#endif	//ARCHICAD

#endif	//SPECKLE_DATABASE_ARCHICAD_DBASE_CORE
