#include "Speckle/Database/Storage/ArchicadDBase/Property/ArchicadPropertyDBaseEngine.h"

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
#include <BM.hpp>

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
	 Make a new template object
	 @param templateData The API template representation
	 @return A new template object (nullptr on failure)
	 */
	Template::Shared makeTemplate(const API_PropertyDefinition& templateData) {
			//NB: This function has been written to allow for the future possibility of different methods for constructing a template and/or
			//failure to make one
		return std::make_shared<Template>(templateData);
	}

}

namespace speckle::database {
	
	class ArchicadPropertyDBaseEngine::Cache : public std::unordered_map<Guid, std::shared_ptr<Template>> {
	public:
		/*!
		 Default constructor
		 */
		Cache() { rebuild(); }
		
		/*!
		 Rebuild the property template cache
		 */
		void rebuild() {
				//Request all Archicad template definitions
			GS::Array<API_PropertyDefinition> definitions;
			if (auto err = ACAPI_Property_GetPropertyDefinitions(APINULLGuid, definitions); err != NoError)
				return;
				//Populate the template cache from the collected definitions
			for (auto iter = definitions.Begin(); iter != definitions.End(); ++iter)
				if (auto propTemplate = makeTemplate(*iter); propTemplate)
					insert({propTemplate->getBIMID(), propTemplate});
		}
	};
	
}

/*--------------------------------------------------------------------
	Constructor

	id: The document storage identifier
	schema: The document storage schema
  --------------------------------------------------------------------*/
ArchicadPropertyDBaseEngine::ArchicadPropertyDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema) :
		ArchicadDBaseCore{id, std::move(schema)} {
} //ArchicadPropertyDBaseEngine::ArchicadPropertyDBaseEngine


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ArchicadPropertyDBaseEngine::~ArchicadPropertyDBaseEngine() {
} //ArchicadPropertyDBaseEngine::~ArchicadPropertyDBaseEngine


/*--------------------------------------------------------------------
	Find all property templates linked to specified classifications
 
	classifications: The classifications
 
	return: A list of shared pointers to linked property templates
  --------------------------------------------------------------------*/
std::vector<std::shared_ptr<Template>> ArchicadPropertyDBaseEngine::findTemplatesByClassification(const BIMRecordIDList& classifications) const {
	std::vector<std::shared_ptr<Template>> result;
	if (validateCache()) {
		for (const auto& templ : *m_cache) {
			for (const auto& classID : classifications) {
				if (templ.second->linksToClassification(classID)) {
					result.push_back(templ.second);
					break;
				}
			}
		}
	}
	return result;
} //ArchicadPropertyDBaseEngine::findTemplatesByClassification


/*--------------------------------------------------------------------
	Get an object by ID

	objID: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)

	return: The requested object (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<Template> ArchicadPropertyDBaseEngine::getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID,
																 std::optional<BIMRecordID> documentID) const {
	if (!validateCache() || (tableID && (tableID != Template::propertyTableID)))
		return nullptr;
	if (auto found = m_cache->find(objID); found != m_cache->end())
		return std::make_unique<Template>(*found->second);
	return nullptr;
} //ArchicadPropertyDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get an object in a transportable form, e.g. packaged for serialisation

	index: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)

	return: The requested wrapped cargo (nullptr on failure)
  --------------------------------------------------------------------*/
active::serialise::Cargo::Unique ArchicadPropertyDBaseEngine::getObjectCargo(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
																			 std::optional<BIMRecordID> documentID)  const {
	return nullptr; //TODO: Implement
} //ArchicadPropertyDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get all objects

	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)

	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Template> ArchicadPropertyDBaseEngine::getObjects(std::optional<BIMRecordID> tableID,
																			std::optional<BIMRecordID> documentID) const {
	return {}; //TODO: Implement
} //ArchicadPropertyDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Get all objects

	filter: The object filter
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)

	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Template> ArchicadPropertyDBaseEngine::getObjects(const Filter& filter, std::optional<BIMRecordID> tableID,
																			std::optional<BIMRecordID> documentID) const {
	return {}; //TODO: Implement
} //ArchicadPropertyDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Write an object to the database

	object: The object to write
	objID: The object ID
	objDocID: The object document-specific ID (unique within a specific document - nullopt if not document-bound)
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
  --------------------------------------------------------------------*/
void ArchicadPropertyDBaseEngine::write(const Template& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID,
										std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadPropertyDBaseEngine::write


/*--------------------------------------------------------------------
	Erase an object by index

	objID: The object ID
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)

	return: True if the object was successfully erased
  --------------------------------------------------------------------*/
void ArchicadPropertyDBaseEngine::erase(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
										std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadPropertyDBaseEngine::erase


/*--------------------------------------------------------------------
	Erase all objects

	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
  --------------------------------------------------------------------*/
void ArchicadPropertyDBaseEngine::erase(std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadPropertyDBaseEngine::erase


/*--------------------------------------------------------------------
	Get the database outline

	return: The database outline
  --------------------------------------------------------------------*/
ArchicadPropertyDBaseEngine::Outline ArchicadPropertyDBaseEngine::getOutline() const {
	return {}; //TODO: Implement
} //ArchicadPropertyDBaseEngine::getOutline


/*--------------------------------------------------------------------
	Handle a project event

	event: The project event

	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ArchicadPropertyDBaseEngine::handle(const event::ProjectEvent& event) {
	using enum ProjectEvent::Type;
	switch (event.getType()) {
		case newDocument: case newAndReset: case open: case close: case quit:
				//Reset the template template cache on any event that changes the active project
			m_cache.reset();
			break;
		default:
			break;
	}
	return false;
} //ArchicadPropertyDBaseEngine::handle


/*--------------------------------------------------------------------
	Ensure the cache is current

	return: True if the cache contains valid te templates
  --------------------------------------------------------------------*/
bool ArchicadPropertyDBaseEngine::validateCache() const {
	if (!m_cache)
		m_cache = std::make_unique<Cache>();
	return !m_cache->empty();
} //ArchicadPropertyDBaseEngine::validateCache

#endif //ARCHICAD
