#include "Speckle/Database/Storage/DocumentStore/DocumentStoreCore.h"

#include "Active/Utility/Defer.h"
#include "Active/Utility/Memory.h"
#include "Active/Utility/String.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Event/Type/DocStoreMergeEvent.h"
#include "Speckle/Event/Type/ProjectEvent.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <BM.hpp>
#endif

using namespace active::event;
using namespace active::setting;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::event;
using namespace speckle::utility;

using enum DocumentStoreCore::Status;

namespace {
	
#ifdef ARCHICAD
	/*--------------------------------------------------------------------
		Convert an Archicad API error code to a DocumentStoreCore status
	 
		acErrorCode: The API error code
	 
		return: An equivalent status code
	 --------------------------------------------------------------------*/
	DocumentStoreCore::Status convertArchicadError(long acErrorCode) {
		using enum DocumentStoreCore::Status;
		switch (acErrorCode) {
			case NoError:
				return nominal;
			case APIERR_BADPARS:
				return badParameter;
			case APIERR_BADID:
				return badID;
			default:
				break;
		}
		return error;
	} //convertArchicadError
	
	
	/*--------------------------------------------------------------------
		Determine if a specified data store exists
	 
		id: The store identity
	 
		return: True if the store exists
	 --------------------------------------------------------------------*/
	bool isExistingStore(active::utility::NameID& storeID) {
		if (storeID.id)
			return true;	//We must have a store if the ID is populated
		GS::UniString storeName{String{storeID.name}};
		API_Guid acID;
		if (auto statusCode = convertArchicadError(ACAPI_AddOnObject_GetObjectGuidFromName(storeName, &acID)); statusCode != nominal)
			throw std::system_error(DocumentStoreCore::makeError(statusCode));
		storeID.id = Guid{acID};
		if (!storeID.id) {
			GS::Array<API_Guid> storedObjects;
			ACAPI_AddOnObject_GetObjectList(&storedObjects);
			for (auto iter = storedObjects.Enumerate(); iter != nullptr; ++iter) {
				API_Guid objGuid = *iter;
				GS::UniString objName;
				GSHandle content = nullptr;
				auto scope = active::utility::defer([&content](){ BMKillHandle(&content); });
				if (ACAPI_AddOnObject_GetObjectContent(objGuid, &objName, &content) == NoError) {
					if (objName == storeName) {
						storeID.id = Guid{objGuid};
						break;
					}
				}
			}
		}
		return storeID.id.operator bool();	//Returns true if the store ID is non-null, i.e. the object exists
	} //isExistingStore

	
	/*--------------------------------------------------------------------
		Copy a GS handle to a Memory object
	 
		handle: The GS handle
		memory: The Memory object to receive the data
	 --------------------------------------------------------------------*/
	void copyHandleToMemory(const GSHandle& handle, active::utility::Memory& memory) {
		auto storeSize = BMGetHandleSize(handle);
		memory.resize(storeSize);
		active::utility::Memory::copy(memory.data(), *handle, storeSize, storeSize);
	} //copyHandleToMemory

#endif

		///Category for DocumentStore processing errors
	class DocumentStoreCategory : public std::error_category {
	public:
			///Category name
		const char* name() const noexcept override {
			return "active::database::sqlite::category";
		}
		/*!
			Get a message for a specified error code
			@param errorCode A DocumentStore processing code
			@return The error message for the specified code
		*/
		std::string message(int errorCode) const override {
				//TODO: These error messages are ok for developers - but can we help users more?
			switch (static_cast<DocumentStoreCore::Status>(errorCode)) {
				case nominal:
					return "";
				case badParameter:
					return "An internal function has been incorrectly used";
				case badID:
					return "Internal data has been requested using an invalid identity";
				case error:
					return "A non-specific error occurred";
				default:
					return "An unknown error occurred";
			}
		}
	};

		///DocumentStore processing category error instance
	static DocumentStoreCategory instance;
	
}

/*--------------------------------------------------------------------
	Make an error code for DocumentStore processing
 
	return: An STL error code
 --------------------------------------------------------------------*/
std::error_code DocumentStoreCore::makeError(DocumentStoreCore::Status code) {
	return std::error_code(static_cast<int>(code), instance);
} //DocumentStoreCore::makeError


/*--------------------------------------------------------------------
	Handle a document merge operation
 
	event: The merge event
 
	return: True if the event should be closed
 --------------------------------------------------------------------*/
bool DocumentStoreCore::handle(const DocStoreMergeEvent& event) {
#ifdef ARCHICAD
	if (event.objects == nullptr)
		return false;
	for (const auto& object : *event.objects) {
		if (*object.name != String{m_id.name})
			continue;
		active::utility::Memory toMerge;
		copyHandleToMemory(object.data, toMerge);
		mergeStore(toMerge);
		writeStore();
	}
#endif
	return false;
} //DocumentStoreCore::handle


/*--------------------------------------------------------------------
	Handle a project event
 
	event: The project event
 
	return: True if the event should be closed
 --------------------------------------------------------------------*/
bool DocumentStoreCore::handle(const event::ProjectEvent& event) {
#ifdef ARCHICAD
	switch (event.getType()) {
		case APINotify_Close:
			resetStore();	//Wipe the cache, forcing a full reload when the data is requested again (after a project is opened)
			break;
		case APINotify_PreSave: case APINotify_SendChanges:
			writeStore();	//Ensure the data is stored with the save/send
			break;
		default:
			break;
	}
#endif
	return false;
} //DocumentStoreCore::handle


/*--------------------------------------------------------------------
	Read the data stored in the document
 
	return: The stored data (empty if the data doesn't exist)
 --------------------------------------------------------------------*/
active::utility::Memory DocumentStoreCore::readStore() const {
	active::utility::Memory result;
		//First establish that we actually have stored data to read
	if (!isExistingStore(m_id))
		return result;
		//Read the stored data
#ifdef ARCHICAD
	GS::UniString storeName{String{m_id.name}};
	GSHandle storedData;
	auto scoped = active::utility::defer([&storedData](){ BMKillHandle(&storedData); });
	if (auto statusCode = convertArchicadError(ACAPI_AddOnObject_GetObjectContent(Guid{m_id.id}, &storeName, &storedData)); statusCode != nominal)
		throw std::system_error(makeError(statusCode));
		//Copy the stored data into the result
	copyHandleToMemory(storedData, result);
#endif
	return result;
} //DocumentStoreCore::readStore


/*--------------------------------------------------------------------
	Write the data to document storage
 
	toWrite: The data to write to storage
 --------------------------------------------------------------------*/
void DocumentStoreCore::writeStore() {
#ifdef ARCHICAD
		//Ensure a suitable data store exists
	if (!isExistingStore(m_id)) {
			//Create when missing
		API_Guid acID;
		if (auto statusCode = convertArchicadError(ACAPI_AddOnObject_CreateUniqueObject(String{m_id.name}, &acID)); statusCode != nominal)
			throw std::system_error(makeError(statusCode));
		m_id.id = Guid{acID};
	}
		//Reserve the storage object in TW
	if (addon()->isSharedDocument()) {
		GS::HashTable<API_Guid, short> conflicts;
		if (auto statusCode = convertArchicadError(ACAPI_AddOnObject_ReserveObjects({Guid{m_id.id}}, &conflicts)); statusCode != nominal)
			throw std::system_error(makeError(statusCode));
			//TODO: Implamenent handling for conflicts
	}
	auto toWrite = buildStore();
		//Write the new data
	GSHandle output = BMAllocateHandle(static_cast<GSSize>(toWrite.size()), ALLOCATE_CLEAR, 0);
	auto scoped = active::utility::defer([&output](){ BMKillHandle(&output); });
	active::utility::Memory::copy(*output, toWrite.data(), toWrite.size(), toWrite.size());
	if (auto statusCode = convertArchicadError(ACAPI_AddOnObject_ModifyObject(Guid{m_id.id}, nullptr, &output)); statusCode != nominal)
		throw std::system_error(makeError(statusCode));
		//Release the storage object in TW
	if (addon()->isSharedDocument()) {
		if (auto statusCode = convertArchicadError(ACAPI_AddOnObject_ReleaseObjects({Guid{m_id.id}})); statusCode != nominal)
			throw std::system_error(makeError(statusCode));
	}
#endif
} //DocumentStoreCore::writeStore


/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
Subscriber::Subscription DocumentStoreCore::subscription() const {
	auto result = DocStoreSubscriber::subscription();
	auto other = ProjectSubscriber::subscription();
	result.insert(other.begin(), other.end());
	return result;
} //DocumentStoreCore::subscription


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool DocumentStoreCore::receive(const active::event::Event& event) {
	return DocStoreSubscriber::receive(event) || ProjectSubscriber::receive(event);
} //DocumentStoreCore::receive


/*--------------------------------------------------------------------
	Attach participant components to the app (as required)
 
	return: True if the participant is able to function
  --------------------------------------------------------------------*/
bool DocumentStoreCore::attach() {
	return DocStoreSubscriber::attach() && ProjectSubscriber::attach();
} //DocumentStoreCore::attach


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool DocumentStoreCore::start() {
	return DocStoreSubscriber::start() && ProjectSubscriber::start();
} //DocumentStoreCore::start
