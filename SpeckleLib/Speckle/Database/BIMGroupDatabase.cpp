#include "Speckle/Database/BIMGroupDatabase.h"

#include "Active/Database/Storage/Storage.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Database/Storage/ArchicadDBase/Property/ArchicadGroupDBaseEngine.h"
#include "Speckle/Record/Property/Setting.h"

#include <array>

using namespace active::container;
using namespace active::database;
using namespace active::event;
using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record;
using namespace speckle::record::property;
using namespace speckle::database;
using namespace speckle::utility;

namespace speckle::database {
	
	///Define other platform engines here as required
#ifdef ARCHICAD
	using GroupDatabaseEngine = ArchicadGroupDBaseEngine;
#endif

		///Group database engine declaration
	class BIMGroupDatabase::Engine : public GroupDatabaseEngine {
		using base = ArchicadGroupDBaseEngine;
		using base::base;
	};

		///Group database storage declaration
	class BIMGroupDatabase::Store : public Storage<Group, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID> {
		using base = Storage<Group, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID>;
		using base::base;
	};
	
}

namespace {
	
		///The database storage identifier for groups
	const char* groupDBaseName = "speckle::database::BIMGroupDatabase";
		///The primary groups table
	const char* groupTableName = "Groups";

}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
BIMGroupDatabase::BIMGroupDatabase() {
	m_engine = std::make_shared<Engine>(groupDBaseName,
			//Schema
		 DBaseSchema{active::utility::String{groupDBaseName},
				//Tables
			{
					//Model group table
				{
					groupTableName, 0, 0, {}
				}
			}
		}
	);
	m_store = std::make_shared<Store>(m_engine);
} //BIMGroupDatabase::BIMGroupDatabase


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BIMGroupDatabase::~BIMGroupDatabase() {}


/*--------------------------------------------------------------------
	Get a specified group
 
	groupID: The ID of the target group
 
	return: The requested group (nullptr on failure)
  --------------------------------------------------------------------*/
Group::Unique BIMGroupDatabase::getGroup(const BIMRecordID& groupID, std::optional<BIMRecordID> tableID,
											   std::optional<BIMRecordID> documentID) const {
	return m_engine->getObject(groupID, tableID, documentID);
} //BIMGroupDatabase::getGroup


/*--------------------------------------------------------------------
	Get a specified group
 
	link: A link to the target group
 
	return: The requested group (nullptr on failure)
  --------------------------------------------------------------------*/
Group::Unique BIMGroupDatabase::getGroup(const BIMLink& link) const {
	return getGroup(link, link.tableID, link.docID);
} //BIMGroupDatabase::getGroup


/*--------------------------------------------------------------------
	Get all groups
 
	return: All the groups
  --------------------------------------------------------------------*/
Vector<Group> BIMGroupDatabase::getGroups() const {
	return m_store->getObjects();
} //BIMGroupDatabase::getGroups


/*--------------------------------------------------------------------
	Write an group to storage
 
	group: The group to write
  --------------------------------------------------------------------*/
void BIMGroupDatabase::write(const Group& group) const {
	m_store->write(group);
} //BIMGroupDatabase::write


/*--------------------------------------------------------------------
	Erase an group
 
	groupID: The ID of the group to erase
  --------------------------------------------------------------------*/
void BIMGroupDatabase::erase(const Guid& groupID) const {
	m_store->erase(groupID);
} //BIMGroupDatabase::erase
