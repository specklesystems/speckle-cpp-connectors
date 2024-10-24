#include "Speckle/Database/Storage/ArchicadDBase/Element/ArchicadElementDBaseEngine.h"

#ifdef ARCHICAD

#include "Active/Utility/Defer.h"
#include "Active/Utility/Memory.h"
#include "Active/Utility/String.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Event/Type/DocStoreMergeEvent.h"
#include "Speckle/Event/Type/ProjectEvent.h"
#include "Speckle/Record/Element/Column.h"
#include "Speckle/Record/Element/ColumnSegment.h"
#include "Speckle/Record/Element/GenericModelElement.h"
#include "Speckle/Record/Element/Beam.h"
#include "Speckle/Record/Element/BeamSegment.h"
#include "Speckle/Record/Element/Memo.h"
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
using namespace speckle::record::element;
using namespace speckle::utility;

using enum ArchicadDBaseCore::Status;

namespace {
	
	/*!
	 Get information about a specified Archicad table
	 @param tableID The ID of the target table
	 @return The requested table info (nullopt on failure)
	 */
	std::optional<API_DatabaseInfo> getTableInfo(const BIMRecordID& tableID) {
		API_DatabaseInfo dbaseInfo;
		dbaseInfo.databaseUnId.elemSetId = tableID;
		if (auto err = ACAPI_Window_GetDatabaseInfo(&dbaseInfo); err == NoError)
			return dbaseInfo;
		return std::nullopt;
	} //getTableInfo

	
	/*!
	 Set the active Archicad table
	 @param tableID The target table ID
	 @return True on success
	 */
	bool setActiveTable(const BIMRecordID& tableID) {
		if (!tableID)
			return false;	//Null guid doens't point to anything
		if (auto activeTable = ArchicadElementDBaseEngine::getActiveTable(); activeTable && *activeTable == tableID)
			return true;
		auto dbaseInfo = getTableInfo(tableID);
		if (!dbaseInfo)
			return false;
		return ACAPI_Database_ChangeCurrentDatabase(&*dbaseInfo) == NoError;
	} //setActiveTable
	
	
	/*!
	 Make a new element object
	 @param elementData The API element representation
	 @param tableID The ID of the parent table (defaults to the active drawing)
	 @return A new element object (nullptr on failure)
	 */
	Element::Unique makeElement(const API_Element& elementData, const BIMRecordID& tableID) {
		switch (elementData.header.type.typeID) {
			case API_ColumnID:
				return std::make_unique<Column>(elementData, tableID);
			case API_ColumnSegmentID:
				return std::make_unique<ColumnSegment>(elementData, tableID);
			case API_BeamID:
				return std::make_unique<Beam>(elementData, tableID);
			case API_BeamSegmentID:
				return std::make_unique<BeamSegment>(elementData, tableID);
			default:
				return std::make_unique<GenericModelElement>(elementData, tableID);
		}
	}
}

/*--------------------------------------------------------------------
	Get the ID of the active Archicad table
 
	return; The active table ID (nullopt on failure)
  --------------------------------------------------------------------*/
std::optional<BIMRecordID> ArchicadElementDBaseEngine::getActiveTable() {
	API_WindowInfo dbaseInfo;
	active::utility::Memory::erase(dbaseInfo);
	if (auto err = ACAPI_Database_GetCurrentDatabase(&dbaseInfo); err == NoError)
		return dbaseInfo.databaseUnId.elemSetId;
	return std::nullopt;
} //ArchicadElementDBaseEngine::getActiveTable


/*--------------------------------------------------------------------
	Get the current user element selection
 
	return: A list of selected element IDs
  --------------------------------------------------------------------*/
BIMLinkList ArchicadElementDBaseEngine::getSelection() const {
	auto tableID = getActiveTable();
	if (!tableID)
		return {};
	BIMLinkList result;
	API_SelectionInfo selectionInfo;
	active::utility::Memory::erase(selectionInfo);
	GS::Array<API_Neig> selection;
	if (auto err = ACAPI_Selection_Get(&selectionInfo, &selection, true); err == NoError) {
		for (const auto& item : selection)
			result.push_back(BIMLink{item, *tableID});
	}
	return result;
} //ArchicadElementDBaseEngine::getSelection


/*--------------------------------------------------------------------
	Get an object by index
 
	index: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: The requested object (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<Element> ArchicadElementDBaseEngine::getObject(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
															   std::optional<BIMRecordID> documentID)  const {
		//Check for memo table requests
	if (tableID == memoTable) {
		auto memo = std::make_unique<API_ElementMemo>();
		active::utility::Memory::erase(*memo);
			//Use memo filtering when requested
		uint64_t filter = documentID ? Guid::toInt(*documentID) : APIMemoMask_All;
		if (auto err = ACAPI_Element_GetMemo(ID, memo.get(), filter); err != NoError)
			ACAPI_DisposeElemMemoHdls(memo.get());
		else {
			auto result = std::make_unique<Memo>();
			result->set(std::move(memo));
			return result;
		}
	}
	if (!tableID) {
			//Use the active table if none is specified
		tableID = getActiveTable();
		if (!tableID)
			return nullptr;
	}
	API_Element element;
	active::utility::Memory::erase(element);
	API_Guid guid{ID.operator API_Guid()};
	if (ACAPI_Element_GetElementFromAnywhere(&guid, &element) != NoError)
		return nullptr;
	return makeElement(element, *tableID);
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
	if (tableID)
		setActiveTable(*tableID);
	return {}; //TODO: Implement
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
