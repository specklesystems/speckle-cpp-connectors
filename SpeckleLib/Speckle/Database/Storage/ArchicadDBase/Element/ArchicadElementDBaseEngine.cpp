#include "Speckle/Database/Storage/ArchicadDBase/Element/ArchicadElementDBaseEngine.h"

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
using namespace speckle::record::element;
using namespace speckle::utility;

using enum ArchicadDBaseCore::Status;

/*--------------------------------------------------------------------
	Get an object by index
 
	index: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: The requested object (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<Element> ArchicadElementDBaseEngine::getObject(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
																		   std::optional<BIMRecordID> documentID)  const {
	return nullptr; //TODO: Implement
} //ArchicadElementDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get an object in a transportable form, e.g. packaged for serialisation
 
	index: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: The requested wrapped cargo (nullptr on failure)
  --------------------------------------------------------------------*/
active::serialise::Cargo::Unique ArchicadElementDBaseEngine::getObjectCargo(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
																		   std::optional<BIMRecordID> documentID)  const {
	return nullptr; //TODO: Implement
} //ArchicadElementDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get all objects
 
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
 
	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Element> ArchicadElementDBaseEngine::getObjects(std::optional<BIMRecordID> tableID,
																					  std::optional<BIMRecordID> documentID) const {
	return {};
} //ArchicadElementDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Get all objects
 
	filter: The object filter
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
 
	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Element> ArchicadElementDBaseEngine::getObjects(const Filter& filter, std::optional<BIMRecordID> tableID,
																					  std::optional<BIMRecordID> documentID) const {
	return {};
} //ArchicadElementDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Write an object to the database
 
	object: The object to write
	objID: The object ID
	objDocID: The object document-specific ID (unique within a specific document - nullopt if not document-bound)
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
  --------------------------------------------------------------------*/
void ArchicadElementDBaseEngine::write(const Element& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID,
																   std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
	
} //ArchicadElementDBaseEngine::write


/*--------------------------------------------------------------------
	Erase an object by index
 
	objID: The object ID
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: True if the object was successfully erased
  --------------------------------------------------------------------*/
void ArchicadElementDBaseEngine::erase(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
													   std::optional<BIMRecordID> documentID) const {

} //ArchicadElementDBaseEngine::erase


/*--------------------------------------------------------------------
	Erase all objects
 
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
  --------------------------------------------------------------------*/
void ArchicadElementDBaseEngine::erase(std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {

} //ArchicadElementDBaseEngine::erase


/*--------------------------------------------------------------------
	Get the database outline
 
	return: The database outline
  --------------------------------------------------------------------*/
ArchicadElementDBaseEngine::Outline ArchicadElementDBaseEngine::getOutline() const {
	return {};
} //ArchicadElementDBaseEngine::getOutline

#endif
