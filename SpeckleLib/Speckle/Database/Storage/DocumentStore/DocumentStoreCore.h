#ifndef SPECKLE_DATABASE_DOCUMENT_STORE_CORE
#define SPECKLE_DATABASE_DOCUMENT_STORE_CORE

#include "Active/File/Path.h"
#include "Active/Setting/SettingList.h"
#include "Active/Database/Storage/DBaseSchema.h"
#include "Active/Utility/NameID.h"
#include "Speckle/Event/Subscriber/DocStoreSubscriber.h"

namespace speckle::database {
	
	/*!
	 Core functionality and definitions for a mechanism to store data in a BIM (3rd-party) document/database
	 
	 Currently implement for Archicad Add-On Objects
	 */
	class DocumentStoreCore : public event::DocStoreSubscriber  {
	public:

		// MARK: - Types
		
			///Status of of the DocumentStore database
		enum class Status {
			nominal,	///<No errors logged
			badParameter,	///<The data supplied to an SDK function call was invalid
			badID,	///<The ID for the stored data is invalid
			error,	///<An unidentified error occurred
		};
		
		// MARK: - Static functions
		
		/*!
		 Make an error code for DocumentStore processing
		 @return An STL error code
		 */
		static std::error_code makeError(DocumentStoreCore::Status code);

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param id The document storage identifier
		 */
		DocumentStoreCore(const active::utility::NameID& id) : m_id(id) {}
		/*!
		 Destructor
		 */
		virtual ~DocumentStoreCore() {}
		
		// MARK: - Function (const)
		
		/*!
		 Get the database id
		 @return The database id
		 */
		const active::utility::NameID& getID() const { return m_id; }
		
		// MARK: - Functions (mutating)
		
		/*!
		 Handle a document merge operation
		 @param event The merge event
		 @return True if the event should be closed
		 */
		bool handle(const event::DocStoreMergeEvent& event) override;
		
	protected:
		/*!
		 Read the data stored in the document (should be lazy-loading, only at the point where data is actually requested)
		 @return The stored data (empty if the data doesn't exist)
		 */
		active::utility::Memory readStore() const;

		/*!
		 Write the data to document storage (currently happens at every change - could implement delayed storage until the point of send/save)
		 */
		void writeStore();
		/*!
		 Build new store data from the latest records
		 @return Data to store from the latest records
		 */
		virtual active::utility::Memory buildStore() = 0;
		/*!
		 Merge existing stored data with incoming stored data (from an external source)
		 @param toMerge The external stored data to merge
		 */
		virtual void mergeStore(const active::utility::Memory& toMerge) = 0;
		/*!
		 Reset the stored data (some external change has invalidated previous data, e.g. the document was closed)
		 */
		virtual void resetStore() = 0;
		
	private:
			///The database ID (mutable to allow lazy loading when data is accessed)
		mutable active::utility::NameID m_id;
	};
	
}

#endif	//SPECKLE_DATABASE_DOCUMENT_STORE_CORE
