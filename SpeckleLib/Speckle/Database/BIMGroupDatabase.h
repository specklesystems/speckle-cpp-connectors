#ifndef CONNECTOR_DATABASE_BIM_GROUP_DATABASE
#define CONNECTOR_DATABASE_BIM_GROUP_DATABASE

#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Property/Group.h"
#include "Speckle/Utility/Guid.h"

namespace active::event {
	class Subscriber;
}

namespace speckle::database {
	
	/*!
	 Database of group templates relating to a specific project
	 
	 Note that this database manages just the group templates, not the values. Group values are attached to elements
	 */
	class BIMGroupDatabase {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		BIMGroupDatabase();
		BIMGroupDatabase(const BIMGroupDatabase&) = delete;
		/*!
		 Destructor
		 */
		~BIMGroupDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get a specified group
		 @param groupID The ID of the target group
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested group (nullptr on failure)
		 */
		record::property::Group::Unique getGroup(const BIMRecordID& groupID, std::optional<BIMRecordID> tableID = std::nullopt,
													   std::optional<BIMRecordID> documentID = std::nullopt) const;
		/*!
		 Get a specified group
		 @param link A link to the target group
		 @return The requested group (nullptr on failure)
		 */
		record::property::Group::Unique getGroup(const BIMLink& link) const;
		/*!
		 Get all model groups
		 @return All the groups
		 */
		active::container::Vector<record::property::Group> getGroups() const;
		/*!
		 Write an group to storage
		 @param group The group to write
		 */
		void write(const record::property::Group& group) const;
		/*!
		 Erase an group
		 @param groupID The ID of the group to erase
		 */
		void erase(const speckle::utility::Guid& groupID) const;
		
	private:
		class Engine;
		class Store;
			///Model group database storage
		std::shared_ptr<Engine> m_engine;
		std::shared_ptr<Store> m_store;
	};

}

#endif	//CONNECTOR_DATABASE_BIM_GROUP_DATABASE
