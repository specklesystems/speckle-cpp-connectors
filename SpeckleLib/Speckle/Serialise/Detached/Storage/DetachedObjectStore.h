#ifndef SPECKLE_SERIALISE_DETACHED_OBJECT_STORE
#define SPECKLE_SERIALISE_DETACHED_OBJECT_STORE

#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Utility/String.h"

namespace speckle::serialise {
	
	/*!
	 Interface for objects that store/cache/send/receive detached objects
	 
	 It is currently assumed that objects will be serialised and can be held in a string. This can be revisited in future if binary data is required.
	 */
	class DetachedObjectStore {
	public:
		
		// MARK: Constructors
		
		/*!
		 Destructor
		 */
		virtual ~DetachedObjectStore() {}
			
		// MARK: - Functions (const)
		
		/*!
		 Retrieve a detached object from storage
		 @param reference The required object reference
		 @return The object data (nullopt if the object cannot be found in storage)
		 */
		virtual utility::String::Option retrieve(const database::RecordID& reference) const = 0;
	
		// MARK: - Functions (mutating)
		
		/*!
		 File a detached object in storage
		 @param reference The object reference (typically a hash generated from the object data)
		 @param data The object data (currently expected to be serialised as JSON)
		 @return True if the object was filed (typically rejected if it duplicates an object already in the store)
		 */
		virtual bool file(const database::RecordID& reference, const utility::String& data) = 0;
	};
	
}

#endif	//SPECKLE_SERIALISE_DETACHED_OBJECT_STORE
