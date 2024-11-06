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
#include "Speckle/Record/Element/MeshElem.h"
#include "Speckle/Record/Element/Morph.h"
#include "Speckle/Record/Element/Roof.h"
#include "Speckle/Record/Element/Shell.h"
#include "Speckle/Record/Element/Slab.h"
#include "Speckle/Record/Element/Wall.h"
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
	
		//ID for the floor plan view
	static const Guid primary2DViewID{String{"ddad27c0-c17b-4ad3-b76b-53d1e176d5ef"}};
		//ID for the 3D view
	static const Guid primary3DViewID{String{"ec368939-fb7d-4d8c-bc88-6d29806d9212"}};

	/*!
	 Get information about a specified Archicad table
	 @param tableID The ID of the target table
	 @return The requested table info (nullopt on failure)
	 */
	std::optional<API_DatabaseInfo> getTableInfo(const BIMRecordID& tableID) {
		API_DatabaseInfo dbaseInfo{};
		if (tableID == primary2DViewID)
			dbaseInfo.typeID = APIWind_FloorPlanID;
		else if (tableID == primary3DViewID)
			dbaseInfo.typeID = APIWind_3DModelID;
		else
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
			return false;	//Null guid doesn't point to anything
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
			case API_MeshID:
				return std::make_unique<Mesh>(elementData, tableID);
			case API_MorphID:
				return std::make_unique<Morph>(elementData, tableID);
			case API_RoofID:
				return std::make_unique<Roof>(elementData, tableID);
			case API_ShellID:
				return std::make_unique<Shell>(elementData, tableID);
			case API_SlabID:
				return std::make_unique<Slab>(elementData, tableID);
			case API_WallID:
				return std::make_unique<Wall>(elementData, tableID);
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
	if (auto err = ACAPI_Database_GetCurrentDatabase(&dbaseInfo); err == NoError) {
		if (dbaseInfo.typeID == APIWind_FloorPlanID)
			return primary2DViewID;
		else if (dbaseInfo.typeID == APIWind_3DModelID)
			return primary3DViewID;
		return dbaseInfo.databaseUnId.elemSetId;
	}
	return std::nullopt;
} //ArchicadElementDBaseEngine::getActiveTable


/*--------------------------------------------------------------------
	Bring the view of this database to the front (i.e. so the user sees it)
 
	tableID: The ID of the table to bring to the front
  --------------------------------------------------------------------*/
void ArchicadElementDBaseEngine::bringViewToFront(BIMRecordID tableID) const {
	auto dbaseInfo = getTableInfo(tableID);
	if (!dbaseInfo)
		return;
	API_WindowInfo windowInfo{};
	windowInfo.typeID = dbaseInfo->typeID;
	if ((windowInfo.typeID != APIWind_FloorPlanID) && (windowInfo.typeID != APIWind_3DModelID))
		windowInfo.databaseUnId = dbaseInfo->databaseUnId;
	ACAPI_Window_ChangeWindow(&windowInfo);
} //ArchicadElementDBaseEngine::bringViewToFront


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
	Set the element selection
  --------------------------------------------------------------------*/
void ArchicadElementDBaseEngine::setSelection(const BIMLinkList& elementIDs) const {
	GS::Array<API_Neig> selNeigs;
	for (const auto& elemID : elementIDs) {
		API_Neig neig(elemID);
		selNeigs.Push(neig);
	}
	ACAPI_Selection_Select(selNeigs, true);
} //ArchicadElementDBaseEngine::setSelection


/*--------------------------------------------------------------------
	Clear the element selection
  --------------------------------------------------------------------*/
void ArchicadElementDBaseEngine::clearSelection() const {
	ACAPI_Selection_DeselectAll();
} //ArchicadElementDBaseEngine::clearSelection


/*--------------------------------------------------------------------
	Get the available dbase tables
 
	targetType: An optional filtr for table type/group to retrieve
 
	return: A list of available tables
  --------------------------------------------------------------------*/
ArchicadElementDBaseEngine::TableIDList ArchicadElementDBaseEngine::getTables(std::optional<TableType> targetType) const {
	using enum ElementStorage::TableType;
	TableIDList result;
	if (!targetType || (targetType == primary2D))
		result.insert(primary2DViewID);
	if (!targetType || (targetType == primary3D))
		result.insert(primary3DViewID);
	return result;
} //ArchicadElementDBaseEngine::getTables


/*--------------------------------------------------------------------
	Find a filtered list of objects
 
	filter: The object filter (nullptr = find all objects)
	tableID: Optional table ID (defaults to the first table)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
 
	return: A list containing IDs of found elements (empty if none found)
  --------------------------------------------------------------------*/
BIMRecordIDList ArchicadElementDBaseEngine::findObjects(const Filter& filter, std::optional<BIMRecordID> tableID,
																		 std::optional<BIMRecordID> documentID) const {
		//Switch to the target table (when specified). Otherwise the currently active table will be used
	if (tableID)
		setActiveTable(*tableID);
		//First check for no filter (in which case we return all objects)
	if (filter == nullptr) {
		GS::Array<API_Guid> found;
		if ((ACAPI_Element_GetElemList({}, &found) != NoError) || found.IsEmpty())
			return {};
		BIMRecordIDList result;
		for (const auto& item : found)
			result.insert(item);
		return result;
	}
		//Implement other filtering as required - ideally identify characteristics supported by API, e.g. filter by type/renovation etc
	return {};
} //ArchicadElementDBaseEngine::findObjects


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
	else {			
			//Use the active table if none is specified
		tableID = getActiveTable();
		if (!tableID)
			return {};
	}
		//Retrieve the element objects to build the result
	active::container::Vector<Element> result;
	auto objectIDs = findObjects();
	for (const auto& ID : objectIDs)
		if (auto element = getObject(ID, tableID); element)
			result.emplace_back(std::move(element));
	return result;
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
