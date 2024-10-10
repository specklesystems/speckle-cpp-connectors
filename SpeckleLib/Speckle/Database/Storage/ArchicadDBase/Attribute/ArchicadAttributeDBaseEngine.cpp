#include "Speckle/Database/Storage/ArchicadDBase/Attribute/ArchicadAttributeDBaseEngine.h"

#ifdef ARCHICAD

#include "Active/Utility/Defer.h"
#include "Active/Utility/Memory.h"
#include "Active/Utility/String.h"
#include "Speckle/Record/Attribute/Finish.h"
#include "Speckle/Record/Attribute/Storey.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Event/Type/DocStoreMergeEvent.h"
#include "Speckle/Event/Type/ProjectEvent.h"
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
using namespace speckle::record::attribute;
using namespace speckle::utility;

using enum ArchicadDBaseCore::Status;

namespace speckle::database {
	
#ifdef ARCHICAD
	/*!
		A class to collect and manage a list of active Archicad storeys
		NB: This list has to be rebuilt every time a storey has changed because Archicad reindexes storeys each time
		This class also has to release memory used by the API to hold storey data
	*/
	class ArchicadAttributeDBaseEngine::StoreyCache : public std::vector<API_StoryType> {
	public:
		/*!
			Constructor (NB: This automatically gathers information about the current storeys)
		*/
		StoreyCache() { rebuild(); }
		
		/*!
			Rebuild the current storey list
		*/
		void rebuild() {
			clear();
			API_StoryInfo storeyInfo;
			active::utility::Memory::erase(storeyInfo);
			ACAPI_ProjectSetting_GetStorySettings(&storeyInfo);
			auto storeyCount = storeyInfo.lastStory - storeyInfo.firstStory + 1;
			for (auto i = 0; i < storeyCount; ++i)
				push_back((*storeyInfo.data)[i]);
			BMKillHandle(reinterpret_cast<GSHandle*>(&storeyInfo.data));
		}
		/*!
			Find a storey by unique ID
			@param id The storey unique ID
			@return An iterator pointing to the found storey (end() on failure)
		*/
		const_iterator find(const Guid& id) const {
			auto floorID = static_cast<short>(Guid::toInt(id));
			return std::find_if(begin(), end(), [&](auto storey){ return storey.floorId == floorID; });
		}
		/*!
			Find a storey by index
			@param index The storey index
			@return An iterator pointing to the found storey (end() on failure)
		*/
		const_iterator find(short index) const {
			return std::find_if(begin(), end(), [&](auto storey){ return storey.index == index; });
		}
	};
#endif
	
}

namespace {
	
		///Attribute factory
	std::unordered_map<active::utility::Guid, std::function<Attribute::Unique(API_Attribute, const active::utility::Guid&)>> attributeFactory;
	
#ifdef ARCHICAD
	/*!
	 Make a new attribute object
	 @param attributeData The API attribute representation
	 @return A new attribute object (nullptr on failure)
	 */
	Attribute::Unique makeAttribute(const API_Attribute& attributeData) {
		auto tableID = active::utility::Guid::fromInt(static_cast<uint64_t>(attributeData.header.typeID));
		if (attributeFactory.empty()) {
			attributeFactory[active::utility::Guid::fromInt(static_cast<uint64_t>(attributeData.header.typeID))] =
					[](API_Attribute attrData, const active::utility::Guid& tableID){ return std::make_unique<Finish>(attrData, tableID); };
		}
		if (auto iter = attributeFactory.find(tableID); iter != attributeFactory.end())
			return iter->second(attributeData, tableID);
		return nullptr;
	}
	
	
	/*!
	 Get the AC API data for a specified attribute
	 @param ID The attribute ID
	 @param tableID The parent table ID
	 @return A new attribute object (nullptr on failure)
	 */
	std::optional<API_Attribute> getAPIData(const BIMRecordID& ID, std::optional<BIMRecordID> tableID) {
		API_Attribute attribute;
		active::utility::Memory::erase(attribute);
		attribute.header.index = ACAPI_CreateAttributeIndex(static_cast<int32_t>(Guid::toInt(ID)));
		attribute.header.typeID = static_cast<API_AttrTypeID>(Guid::toInt(*tableID));
		if (ACAPI_Attribute_Get(&attribute) != NoError)
			return std::nullopt;
		return attribute;
	}
#endif
	
}

/*--------------------------------------------------------------------
	Constructor
 
	id: The document storage identifier
  --------------------------------------------------------------------*/
ArchicadAttributeDBaseEngine::ArchicadAttributeDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema) :
		ArchicadDBaseCore{id, std::move(schema)} {
} //ArchicadAttributeDBaseEngine::ArchicadAttributeDBaseEngine


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ArchicadAttributeDBaseEngine::~ArchicadAttributeDBaseEngine() {
} //ArchicadAttributeDBaseEngine::~ArchicadAttributeDBaseEngine


/*--------------------------------------------------------------------
	Get an object by ID
 
	objID: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: The requested object (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<Attribute> ArchicadAttributeDBaseEngine::getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID,
																   std::optional<BIMRecordID> documentID) const {
	if (!tableID)
		return nullptr;
	if (auto attrData = ::getAPIData(objID, *tableID); attrData)
		return makeAttribute(*attrData);
	return nullptr;
} //ArchicadAttributeDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get an object in a transportable form, e.g. packaged for serialisation
 
	index: The object index
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: The requested wrapped cargo (nullptr on failure)
  --------------------------------------------------------------------*/
active::serialise::Cargo::Unique ArchicadAttributeDBaseEngine::getObjectCargo(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
																		   std::optional<BIMRecordID> documentID)  const {
	return nullptr; //TODO: Implement
} //ArchicadAttributeDBaseEngine::getObject


/*--------------------------------------------------------------------
	Get all objects
 
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
 
	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Attribute> ArchicadAttributeDBaseEngine::getObjects(std::optional<BIMRecordID> tableID,
																					  std::optional<BIMRecordID> documentID) const {
	return {}; //TODO: Implement
} //ArchicadAttributeDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Get all objects
 
	filter: The object filter
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
 
	return: The requested objects (nullptr on failure)
  --------------------------------------------------------------------*/
active::container::Vector<Attribute> ArchicadAttributeDBaseEngine::getObjects(const Filter& filter, std::optional<BIMRecordID> tableID,
																					  std::optional<BIMRecordID> documentID) const {
	return {}; //TODO: Implement
} //ArchicadAttributeDBaseEngine::getObjects


/*--------------------------------------------------------------------
	Write an object to the database
 
	object: The object to write
	objID: The object ID
	objDocID: The object document-specific ID (unique within a specific document - nullopt if not document-bound)
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
  --------------------------------------------------------------------*/
void ArchicadAttributeDBaseEngine::write(const Attribute& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID,
																   std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadAttributeDBaseEngine::write


/*--------------------------------------------------------------------
	Erase an object by index
 
	objID: The object ID
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (when the object is bound to a specific document)
 
	return: True if the object was successfully erased
  --------------------------------------------------------------------*/
void ArchicadAttributeDBaseEngine::erase(const BIMRecordID& ID, std::optional<BIMRecordID> tableID,
													   std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadAttributeDBaseEngine::erase


/*--------------------------------------------------------------------
	Erase all objects
 
	tableID: Optional table ID (defaults to the floor plan)
	documentID: Optional document ID (filter for this document only - nullopt = all objects)
  --------------------------------------------------------------------*/
void ArchicadAttributeDBaseEngine::erase(std::optional<BIMRecordID> tableID, std::optional<BIMRecordID> documentID) const {
		//TODO: Implement
} //ArchicadAttributeDBaseEngine::erase


/*--------------------------------------------------------------------
	Get the database outline
 
	return: The database outline
  --------------------------------------------------------------------*/
ArchicadAttributeDBaseEngine::Outline ArchicadAttributeDBaseEngine::getOutline() const {
	return {}; //TODO: Implement
} //ArchicadAttributeDBaseEngine::getOutline


/*--------------------------------------------------------------------
	Get attribute data direct from the AC API. For internal use - avoid direct use
 
	link: A link to the required attribute
 
	return: The AC API attribute data
  --------------------------------------------------------------------*/
std::optional<API_Attribute> ArchicadAttributeDBaseEngine::getAPIData(const BIMLink& link) const {
	return ::getAPIData(link, link.tableID);
} //ArchicadAttributeDBaseEngine::getAPIData


/*--------------------------------------------------------------------
	Get storey data direct from the AC API. For internal use - avoid direct use
 
	link: A link to the required storey
 
	return: The AC API storey data
  --------------------------------------------------------------------*/
std::optional<API_StoryType> ArchicadAttributeDBaseEngine::getAPIStorey(const BIMLink& link) const {
	if (!m_storeyCache)
		m_storeyCache = std::make_unique<StoreyCache>();
	if (auto iter = m_storeyCache->find(link); iter != m_storeyCache->end())
		return *iter;
	return std::nullopt;
} //ArchicadAttributeDBaseEngine::getAPIStorey

#endif
