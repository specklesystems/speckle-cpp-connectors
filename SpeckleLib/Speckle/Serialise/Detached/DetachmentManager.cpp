#include "Speckle/Serialise/Detached/DetachmentManager.h"

#include "Active/Utility/BufferOut.h"
#include "Speckle/Serialise/Detached/DetachedWrap.h"
#include "Speckle/Serialise/Detached/Storage/DetachedObjectStore.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::serialise;
using namespace speckle::utility;


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
DetachmentManager::DetachmentManager() {
	
} //DetachmentManager::DetachmentManager


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
DetachmentManager::~DetachmentManager() {
	
} //DetachmentManager::~DetachmentManager


/*--------------------------------------------------------------------
	Send cargo as a detached object
 
	package: The package to become a detached object
	identity: The package identity (name, optional namespace)
 
	return: The ID of the sent detached object
  --------------------------------------------------------------------*/
std::optional<RecordID> DetachmentManager::send(Package&& package, const Identity& identity) const {
	auto transport = m_transport.lock();
	if (!transport)
		throw;	//TODO: Throw meaningful exception type
		//Prepare the detached object data
	String serialisedData;
	active::utility::BufferOut buffer{serialisedData};
		//The wrapper will manage the object serialisation and generate a unique reference based on the output
	DetachedWrap wrapper{package, buffer};
	transport->send(std::forward<Cargo&&>(wrapper), identity, serialisedData);
	auto detachedReference = wrapper.getReference();
		//If a reference is obtained, file/cache the object as required
	if (detachedReference) {
		m_store->file(*detachedReference, serialisedData);
		if (m_cache)
			m_cache->file(*detachedReference, serialisedData);
	}
	return detachedReference;
} //DetachmentManager::send


/*--------------------------------------------------------------------
	Receive cargo from a detached object
 
	id: The ID of the detached object to receive
	package: The package to receive the detached object data
	identity: The package identity (name, optional namespace)
 
	return: True if the detached object was found and received
  --------------------------------------------------------------------*/
bool DetachmentManager::receive(const database::RecordID& id, Package&& package, const Identity& identity) const {
	return false;	//TODO: Implement when we start receiving in Archicad
} //DetachmentManager::receive


/*--------------------------------------------------------------------
	Set the manager primary storage
 
	store: the primary storage (this is the long-term storage, e.g. a remote/cloud database)
  --------------------------------------------------------------------*/
void DetachmentManager::setStore(std::unique_ptr<DetachedObjectStore> store) {
	m_store = std::move(store);
} //DetachmentManager::setStore


/*--------------------------------------------------------------------
	Set the manager storage cache
 
	cache: the storage cache (this is intended to provide faster access than the primary storage for repeated detached objects)
  --------------------------------------------------------------------*/
void DetachmentManager::setCache(std::unique_ptr<DetachedObjectStore> cache) {
	m_cache = std::move(cache);
} //DetachmentManager::setCache
