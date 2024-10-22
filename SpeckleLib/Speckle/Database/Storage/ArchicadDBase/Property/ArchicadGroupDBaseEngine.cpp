#include "Speckle/Database/Storage/ArchicadDBase/Property/ArchicadGroupDBaseEngine.h"

#ifdef ARCHICAD

#include "Active/Utility/Memory.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Event/Type/ProjectEvent.h"
#include "Speckle/Record/Property/Setting.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <ACAPinc.h>
#include <ACAPI_Database.h>

using namespace active::event;
using namespace active::setting;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::event;
using namespace speckle::record::property;
using namespace speckle::utility;

using enum ArchicadDBaseCore::Status;

namespace {
	
	/*!
	 Make a new group object
	 @param groupData The API group representation
	 @return A new group object (nullptr on failure)
	 */
	Group::Shared makeGroup(const API_PropertyGroup& groupData) {
			//NB: This function has been written to allow for the future possibility of different methods for constructing a group and/or
			//failure to make one
		return std::make_shared<Group>(groupData);
	}

}

namespace speckle::database {
	
	class ArchicadGroupDBaseEngine::Cache : public std::unordered_map<Guid, std::shared_ptr<Group>> {
	public:
		/*!
		 Default constructor
		 */
		Cache() { rebuild(); }
		
		/*!
		 Rebuild the property group cache
		 */
		void rebuild() {
				//Request all Archicad group groups
			GS::Array<API_PropertyGroup> groups;
			if (auto err = ACAPI_Property_GetPropertyGroups(groups); err != NoError)
				return;
				//Populate the group cache from the collected groups
			for (auto iter = groups.Begin(); iter != groups.End(); ++iter)
				if (auto propGroup = makeGroup(*iter); propGroup)
					insert({propGroup->getBIMID(), propGroup});
		}
	};
	
}

/*--------------------------------------------------------------------
	Constructor

	id: The document storage identifier
	schema: The document storage schema
  --------------------------------------------------------------------*/
ArchicadGroupDBaseEngine::ArchicadGroupDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema) :
		ArchicadDBaseCore{id, std::move(schema)} {
} //ArchicadGroupDBaseEngine::ArchicadGroupDBaseEngine


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ArchicadGroupDBaseEngine::~ArchicadGroupDBaseEngine() {
} //ArchicadGroupDBaseEngine::~ArchicadGroupDBaseEngine


/*--------------------------------------------------------------------
	Get an object by ID

	objID: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)

	return: The requested object (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<Group> ArchicadGroupDBaseEngine::getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID,
																 std::optional<BIMRecordID> documentID) const {
	if (!validateCache() || (tableID && (tableID != Group::propertyGroupTableID)))
		return nullptr;
	if (auto found = m_cache->find(objID); found != m_cache->end())
		return std::make_unique<Group>(*found->second);
	return nullptr;
} //ArchicadGroupDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get an object in a transportable form, e.g. packaged for serialisation

	index: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)

	return: The requested wrapped cargo (nullptr on failure)
  --------------------------------------------------------------------*/
active::serialise::Cargo::Unique ArchicadGroupDBaseEngine::getObjectCargo(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
																			 std::optional<BIMRecordID> documentID)  const {
	return nullptr; //TODO: Implement
} //ArchicadGroupDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get all objects

	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)

	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Group> ArchicadGroupDBaseEngine::getObjects(std::optional<BIMRecordID> tableID,
																			std::optional<BIMRecordID> documentID) const {
	return {}; //TODO: Implement
} //ArchicadGroupDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Get all objects

	filter: The object filter
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)

	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Group> ArchicadGroupDBaseEngine::getObjects(const Filter& filter, std::optional<BIMRecordID> tableID,
																			std::optional<BIMRecordID> documentID) const {
	return {}; //TODO: Implement
} //ArchicadGroupDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Write an object to the database

	object: The object to write
	objID: The object ID
	objDocID: The object document-specific ID (unique within a specific document - nullopt if not document-bound)
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
  --------------------------------------------------------------------*/
void ArchicadGroupDBaseEngine::write(const Group& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID,
										std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadGroupDBaseEngine::write


/*--------------------------------------------------------------------
	Erase an object by index

	objID: The object ID
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)

	return: True if the object was successfully erased
  --------------------------------------------------------------------*/
void ArchicadGroupDBaseEngine::erase(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
										std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadGroupDBaseEngine::erase


/*--------------------------------------------------------------------
	Erase all objects

	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
  --------------------------------------------------------------------*/
void ArchicadGroupDBaseEngine::erase(std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadGroupDBaseEngine::erase


/*--------------------------------------------------------------------
	Get the database outline

	return: The database outline
  --------------------------------------------------------------------*/
ArchicadGroupDBaseEngine::Outline ArchicadGroupDBaseEngine::getOutline() const {
	return {}; //TODO: Implement
} //ArchicadGroupDBaseEngine::getOutline


/*--------------------------------------------------------------------
	Handle a project event

	event: The project event

	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ArchicadGroupDBaseEngine::handle(const event::ProjectEvent& event) {
	using enum ProjectEvent::Type;
	switch (event.getType()) {
		case newDocument: case newAndReset: case open: case close: case quit:
				//Reset the group cache on any event that changes the active project
			m_cache.reset();
			break;
		default:
			break;
	}
	return false;
} //ArchicadGroupDBaseEngine::handle


/*--------------------------------------------------------------------
	Ensure the cache is current

	return: True if the cache contains valid te groups
  --------------------------------------------------------------------*/
bool ArchicadGroupDBaseEngine::validateCache() const {
	if (!m_cache)
		m_cache = std::make_unique<Cache>();
	return !m_cache->empty();
} //ArchicadGroupDBaseEngine::validateCache

#endif //ARCHICAD
