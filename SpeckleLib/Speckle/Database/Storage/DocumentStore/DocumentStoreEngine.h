#ifndef SPECKLE_DATABASE_DOCUMENT_STORE_ENGINE
#define SPECKLE_DATABASE_DOCUMENT_STORE_ENGINE

#include "Active/Database/Storage/DBaseEngine.h"
#include "Active/Database/Storage/RecordCache.h"
#include "Active/File/Path.h"
#include "Active/Serialise/Cargo.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Serialise/Transport.h"
#include "Active/Utility/BufferIn.h"
#include "Speckle/Database/Storage/DocumentStore/DocumentStoreCore.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <algorithm>
#include <ranges>

namespace speckle::database {
	
		///Concept for the ability to store objects in a document
	template<typename Obj, typename Transport>
	concept DocumentStorable = std::is_base_of_v<active::serialise::Cargo, Obj> &&
			std::is_base_of_v<active::serialise::Transport, Transport>;

	/*!
	 A database engine to store records in a 3rd-party BIM document
	 
	 Due to the fact that these records are intended to represent a single table and are stored in a document, the concepts of 'table' and
	 'document' aren't currently applicable (this could be extended in future if there is a use-case)
	 @tparam Obj Interface for the stored object. NB: This can be a base class for an object hierarchy, not necessarily a concrete class
	 @tparam Transport The serialisation transport mechanism for objects
	 @tparam ObjID The object identifier type, e.g. Guid
	 */
	template<typename Obj, typename Transport, typename ObjID = utility::String>
	requires DocumentStorable<Obj, Transport>
	class DocumentStoreEngine : public DocumentStoreCore, public active::database::DBaseEngine<Obj, ObjID, utility::String, utility::String>  {
	public:
		
		// MARK: - Types
		
		using base = active::database::DBaseEngine<Obj, ObjID, utility::String, utility::String>;
		using Filter = base::Filter;
		using Cache = active::database::RecordCache<Obj, ObjID>;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param id The document storage identifier
		 */
		DocumentStoreEngine(const active::utility::NameID& id) : DocumentStoreCore{id} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		DocumentStoreEngine(const DocumentStoreEngine& source) : DocumentStoreCore{source}, base{source} {
			m_cache = (source.m_cache) ? std::make_unique<Cache>(*source.m_cache) : nullptr;
		}
		/*!
			Object cloning
			@return A clone of this object
		*/
		DocumentStoreEngine* clonePtr() const override { return new DocumentStoreEngine{*this}; }
		
		// MARK: - Functions (const)
		
		/*!
		 Get an object by index
		 @param ID The object ID
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested object (nullptr on failure)
		 */
		std::unique_ptr<Obj> getObject(const ObjID& ID, utility::String::Option tableID = std::nullopt, utility::String::Option documentID = std::nullopt) const override;
		/*!
		 Get all objects
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The requested objects (nullptr on failure)
		 */
		active::container::Vector<Obj> getObjects(utility::String::Option tableID = std::nullopt, utility::String::Option documentID = std::nullopt) const override;
		/*!
		 Get a filtered list of objects
		 @param filter The object filter
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The filtered objects (nullptr on failure)
		 */
		active::container::Vector<Obj> getObjects(const Filter& filter, utility::String::Option tableID = std::nullopt,
												  utility::String::Option documentID = std::nullopt) const override;
		/*!
		 Erase an object by index
		 @param ID The object ID
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(const ObjID& ID, utility::String::Option tableID = std::nullopt, utility::String::Option documentID = std::nullopt) const override;
		/*!
		 Erase all objects
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(utility::String::Option tableID = std::nullopt, utility::String::Option documentID = std::nullopt) const override;
	protected:
		/*!
		 Get the cache of records built from the data stored in the document
		 @return The cached records
		 */
		Cache* getCache() const;
		/*!
		 Build new store data from the latest records
		 @return Data to store from the latest records
		 */
		active::utility::Memory buildStore() override;
		/*!
		 Merge existing stored data with incoming stored data (from an external source)
		 @param toMerge The external stored data to merge
		 @return The merged data to be stored
		 */
		void mergeStore(const active::utility::Memory& toMerge) override;
		/*!
		 Reset the stored data (some external change has invalidated previous data, e.g. the document was closed)
		 */
		void resetStore() override { m_cache.reset(); }
		
	private:
			//Cached records from the document store
		std::unique_ptr<Cache> m_cache;
	};
	
	/*--------------------------------------------------------------------
		Get the cache of records built from the data stored in the document
	 
		return: The cached records (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	typename DocumentStoreEngine<Obj, Transport, ObjID>::Cache* DocumentStoreEngine<Obj, Transport, ObjID>::getCache() const {
		if (m_cache)
			return m_cache;
			//Read the data stored in the document
		auto storedData = readStore();
		m_cache = std::make_unique<Cache>();
		if (!storedData)
			return m_cache;	//Return an empty container if there's no data
			//Import the document data into the record cache
		Transport().receive(std::forward<active::serialise::Cargo&&>(*m_cache), active::serialise::Identity{}, storedData);
		return m_cache;
	} //DocumentStoreEngine<Obj, Transport, ObjID>::getCache
	
	
	/*--------------------------------------------------------------------
		Get an object by index
	 
		index: The object index
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (when the object is bound to a specific document)
	 
		return: The requested object (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	std::unique_ptr<Obj> DocumentStoreEngine<Obj, Transport, ObjID>::getObject(const ObjID& ID, utility::String::Option tableID,
																			   utility::String::Option documentID)  const {
		auto cache = getCache();
		return cache ? cache->read(ID) : nullptr;
	} //DocumentStoreEngine<Obj, Transport, ObjID>::getObject
	
	
	/*--------------------------------------------------------------------
		Get all objects
	 
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (filter for this document only - nullopt = all objects)
	 
		return: The requested objects (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	active::container::Vector<Obj> DocumentStoreEngine<Obj, Transport, ObjID>::getObjects(utility::String::Option tableID,
																						  utility::String::Option documentID) const {
		auto cache = getCache();
		return cache ? cache->read() : nullptr;
	} //DocumentStoreEngine<Obj, Transport, ObjID>::getObjects
	
	
	/*--------------------------------------------------------------------
		Get all objects
	 
		filter: The object filter
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (filter for this document only - nullopt = all objects)
	 
		return: The requested objects (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	active::container::Vector<Obj> DocumentStoreEngine<Obj, Transport, ObjID>::getObjects(const Filter& filter, utility::String::Option tableID,
																						  utility::String::Option documentID) const {
		auto cache = getCache();
		return cache ? cache->read(filter) : nullptr;
	} //DocumentStoreEngine<Obj, Transport, ObjID>::getObjects
	
	
	/*--------------------------------------------------------------------
		Erase an object by index
	 
		objID: The object ID
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (when the object is bound to a specific document)
	 
		return: True if the object was successfully erased
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, Transport, ObjID>::erase(const ObjID& ID, utility::String::Option tableID,
														   utility::String::Option documentID) const {
		auto cache = getCache();
		if (cache)
			cache->erase(ID);
	} //DocumentStoreEngine<Obj, Transport, ObjID>::erase
	
	
	/*--------------------------------------------------------------------
		Erase all objects
	 
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (filter for this document only - nullopt = all objects)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, Transport, ObjID>::erase(utility::String::Option tableID, utility::String::Option documentID) const {
		auto cache = getCache();
		if (cache)
			cache->erase();
	} //DocumentStoreEngine<Obj, Transport, ObjID>::erase
	
	
	/*--------------------------------------------------------------------
		Build new store data from the latest records
	 
		return: Data to store from the latest records
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	active::utility::Memory DocumentStoreEngine<Obj, Transport, ObjID>::buildStore() {
		active::utility::Memory result;
		auto cache = getCache();
		if (!cache)
			cache = std::make_unique<Cache>();	//We still want to export an empty cache object even if it contains no records
			//Export the cached records
		Transport().send(std::forward<active::serialise::Cargo&&>(*m_cache), active::serialise::Identity{}, result);
		return result;
	} //DocumentStoreEngine<Obj, Transport, ObjID>::buildStore
	
	
	/*--------------------------------------------------------------------
		Merge existing stored data with incoming stored data (from an external source)
	 
		toMerge: The external stored data to merge
	 
		return: The merged data to be stored
	  --------------------------------------------------------------------*/
	template<typename Obj, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, Transport, ObjID>::mergeStore(const active::utility::Memory& toMerge) {
			//Import the incoming records from the data to merge
		Cache incoming;
		Transport().receive(std::forward<active::serialise::Cargo&&>(incoming), active::serialise::Identity{}, toMerge);
			//Get the data currently stored in the document
		auto cache = getCache();
		if (!cache)
			cache = std::make_unique<Cache>();	//We still want to export an empty cache object even if it contains no records
		cache->merge(incoming);
	} //DocumentStoreEngine<Obj, Transport, ObjID>::mergeStore
	
}

#endif	//SPECKLE_DATABASE_DOCUMENT_STORE_ENGINE
