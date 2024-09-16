#ifndef SPECKLE_DATABASE_DOCUMENT_STORE_CORE
#define SPECKLE_DATABASE_DOCUMENT_STORE_CORE

#include "Active/File/Path.h"
#include "Active/Setting/SettingList.h"
#include "Active/Database/Storage/DBaseSchema.h"
#include "Active/Utility/NameID.h"
#include "Speckle/Event/Subscriber/DocStoreSubscriber.h"
#include "Speckle/Event/Subscriber/ProjectSubscriber.h"

namespace speckle::database {
	
	using DocumentStoreSchema = active::database::DBaseSchema<>;

	/*!
	 Core functionality and definitions for a mechanism to store data in a BIM (3rd-party) document/database
	 
	 Currently implement for Archicad Add-On Objects
	 */
	class DocumentStoreCore : public event::DocStoreSubscriber, public event::ProjectSubscriber  {
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
		 @param schema The document storage schema
		 */
		DocumentStoreCore(const active::utility::NameID& id, DocumentStoreSchema&& schema) : m_id(id), m_schema{schema} {}
		DocumentStoreCore(const DocumentStoreCore&) = default;
		/*!
		 Destructor
		 */
		virtual ~DocumentStoreCore() {}
		
		// MARK: - Function (const)
		
		/*!
		 Get the database schema
		 @return The database schema
		 */
		const DocumentStoreSchema& getSchema() const { return m_schema; }
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
		/*!
		 Handle a project event
		 @param event The project event
		 @return True if the event should be closed
		 */
		bool handle(const event::ProjectEvent& event) override;
		
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
		virtual void resetStore() { m_id.id.clear(); }

		// MARK: Subscriber functions
		
		/*!
		 Get the event subscription list
		 @return The subscription list (an empty list will put the subscriber into a suspended state)
		*/
		Subscription subscription() const override;
		/*!
		 Receive a subscribed event
		 @param event The incoming event
		 @return True if the event should be closed
		 */
		bool receive(const active::event::Event& event) override;
		/*!
		 Attach participant components to the app (as required)
		 @return True if the participant is able to function
		*/
		bool attach() override;
		/*!
		 Start the participant operation
		 @return True if the participant is able to continue
		 */
		bool start() override;
		
	private:
			///The database schema
		DocumentStoreSchema m_schema;
			///The database ID (mutable to allow lazy loading when data is accessed)
		mutable active::utility::NameID m_id;
	};
	
}

#endif	//SPECKLE_DATABASE_DOCUMENT_STORE_CORE
