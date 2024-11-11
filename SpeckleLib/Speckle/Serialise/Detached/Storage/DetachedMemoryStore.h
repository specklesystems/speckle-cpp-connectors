#ifndef SPECKLE_SERIALISE_DETACHED_MEMORY_STORE
#define SPECKLE_SERIALISE_DETACHED_MEMORY_STORE

#include "Speckle/Serialise/Detached/Storage/DetachedObjectStore.h"

#include <unordered_map>

namespace speckle::serialise {
	
	/*!
	 Memory-based storage for detached objects
	 
	 Detached object serialised data is held in a simple unordered_map keying the data against the object reference.
	 This class primarily intended for collecting the JSON from detached objects and holding until they can be forwarded to the server. The data
	 will not persist beyond this usage.
	 */
	class DetachedMemoryStore : public DetachedObjectStore, public std::unordered_map<database::RecordID, utility::String> {
	public:
		
		// MARK: - Types
		
		using base = std::unordered_map<database::RecordID, utility::String>;
			
		// MARK: - Functions (const)
		
		/*!
		 Retrieve a detached object from storage
		 @param reference The required object reference
		 @return The object data (nullopt if the object cannot be found in storage)
		 */
		utility::String::Option retrieve(const database::RecordID& reference) const override {
			if (auto iter = base::find(reference); iter != base::end())
				return iter->second;
			return std::nullopt;
		}
	
		// MARK: - Functions (mutating)
		
		/*!
		 File a detached object in storage
		 @param reference The object reference (typically a hash generated from the object data)
		 @param data The object data (currently expected to be serialised as JSON)
		 @return True if the object was filed (typically ignored if it duplicates an object already in the store)
		 */
		bool file(const database::RecordID& reference, const utility::String& data) override {
			return base::insert(std::make_pair(reference, data)).second;
		}
	};
	
}

#endif	//SPECKLE_SERIALISE_DETACHED_MEMORY_STORE
