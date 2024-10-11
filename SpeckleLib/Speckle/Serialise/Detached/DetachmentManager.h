#ifndef SPECKLE_SERIALISE_DETACHMENT_MANAGER
#define SPECKLE_SERIALISE_DETACHMENT_MANAGER

#include "Active/Serialise/Management/Manager.h"
#include "Active/Serialise/Transport.h"
#include "Speckle/Database/Identity/RecordID.h"

namespace active::serialise {
	class Package;
}

namespace speckle::serialise {
	
	class DetachedObjectStore;
	
	/*!
	 Manager for detached (serialised) objects
	 
	 Pass this manager to the target transport for (de)serialisation supporting detached objects. Note that the manager is expected to know how
	 to send/receive detached objects, e.g. to a JS portal, remote server or local database (or some combo)
	 */
	class DetachmentManager : public virtual active::serialise::Manager {
	public:
		
		// MARK: - Types
		
			///Base class
		using base = active::serialise::Manager;
		
		/*!
		 Default constructor
		 */
		DetachmentManager();
		DetachmentManager(const DetachmentManager&) = delete;
		/*!
		 Destructor
		 */
		~DetachmentManager();
			
		// MARK: - Functions (const)
		
		/*!
			Send cargo as a detached object
			@param package The package to become a detached object
			@param identity The package identity (name, optional namespace)
			@return The ID of the sent detached object
		*/
		std::optional<database::RecordID> send(active::serialise::Package&& package, const active::serialise::Identity& identity) const;
		/*!
			Receive cargo from a detached object
			@param id The ID of the detached object to receive
			@param package The package to receive the detached object data
			@param identity The package identity (name, optional namespace)
			@return True if the detached object was found and received
		*/
		bool receive(const database::RecordID& id, active::serialise::Package&& package, const active::serialise::Identity& identity) const;
		
	// MARK: - Functions (mutating)

		/*!
			Set the manager primary storage
			@param store the primary storage (this is the long-term storage, e.g. a remote/cloud database)
		*/
		void setStore(std::unique_ptr<DetachedObjectStore> store);
		/*!
			Set the manager storage cache
			@param cache the storage cache (this is intended to provide faster access than the primary storage for repeated detached objects)
		*/
		void setCache(std::unique_ptr<DetachedObjectStore> cache);

	private:
			///The manager serialisation transport (NB - this is expected to be a shared resource, so the manager does not own it)
		std::weak_ptr<active::serialise::Transport> m_transport;
			///The primary store for sending/retrieving detached objects
		std::unique_ptr<DetachedObjectStore> m_store;
			///A cache for detached objects - can be used as a local store to save
		std::unique_ptr<DetachedObjectStore> m_cache;
	};
	
}

#endif	//SPECKLE_SERIALISE_DETACHMENT_MANAGER
