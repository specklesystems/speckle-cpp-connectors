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
#include "Active/Utility/BufferOut.h"
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
	 @tparam ObjWrapper Wrapper type for (de)serialising objects
	 @tparam Transport The serialisation transport mechanism for objects
	 @tparam ObjID The object identifier type, e.g. Guid
	 */
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID = active::utility::String>
	requires DocumentStorable<Obj, Transport>
	class DocumentStoreEngine : public DocumentStoreCore, public active::database::DBaseEngine<Obj, ObjID, active::utility::String, active::utility::String>  {
	public:
		
		// MARK: - Types
		
		using base = active::database::DBaseEngine<Obj, ObjID, utility::String, active::utility::String>;
		using Filter = base::Filter;
		using Outline = base::Outline;
		using Cache = active::database::RecordCache<Obj, ObjID, active::utility::String, active::utility::String>;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param id The document storage identifier
		 */
		DocumentStoreEngine(const active::utility::NameID& id, DocumentStoreSchema&& schema) : DocumentStoreCore{id, std::move(schema)} {}
		DocumentStoreEngine(const DocumentStoreEngine&) = delete;
		
		// MARK: - Functions (const)
		
		/*!
		 Get an object by index
		 @param ID The object ID
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested object (nullptr on failure)
		 */
		std::unique_ptr<Obj> getObject(const ObjID& objID, std::optional<active::utility::String> tableID = std::nullopt, std::optional<active::utility::String> documentID = std::nullopt) const override;
		/*!
		 Get all objects
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The requested objects (nullptr on failure)
		 */
		active::container::Vector<Obj> getObjects(std::optional<active::utility::String> tableID = std::nullopt, std::optional<active::utility::String> documentID = std::nullopt) const override;
		/*!
		 Get a filtered list of objects
		 @param filter The object filter
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The filtered objects (nullptr on failure)
		 */
		active::container::Vector<Obj> getObjects(const Filter& filter, std::optional<active::utility::String> tableID = std::nullopt,
												  std::optional<active::utility::String> documentID = std::nullopt) const override;
		/*!
		 Write an object to the database
		 @param object The object to write
		 @param objID The object ID
		 @param objDocID The object document-specific ID (unique within a specific document - nullopt if not document-bound)
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 */
		void write(const Obj& object, const ObjID& objID, std::optional<ObjID> objDocID = std::nullopt,
				   utility::String::Option tableID = std::nullopt, utility::String::Option documentID = std::nullopt) const override;
		/*!
		 Erase an object by index
		 @param ID The object ID
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(const ObjID& ID, std::optional<active::utility::String> tableID = std::nullopt,
				   std::optional<active::utility::String> documentID = std::nullopt) const override;
		/*!
		 Erase all objects
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(std::optional<active::utility::String> tableID = std::nullopt, std::optional<active::utility::String> documentID = std::nullopt) const override;
		/*!
		 Get the database outline
		 @return The database outline
		 */
		Outline getOutline() const override;
		
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
		mutable std::unique_ptr<Cache> m_cache;
	};
	
	/*--------------------------------------------------------------------
		Get the cache of records built from the data stored in the document
	 
		return: The cached records (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	typename DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::Cache* DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getCache() const {
		if (m_cache)
			return m_cache.get();
			//Read the data stored in the document
		auto storedData = readStore();
		m_cache = std::make_unique<Cache>();
		if (!storedData)
			return m_cache.get();	//Return an empty container if there's no data
			//Import the document data into the record cache
		if constexpr (std::is_same_v<ObjWrapper, Obj>)
			Transport().receive(std::forward<active::serialise::Cargo&&>(*m_cache), active::serialise::Identity{}, storedData);
		else
			Transport().receive(ObjWrapper{*m_cache}, active::serialise::Identity{}, storedData);
		return m_cache.get();
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getCache
	
	
	/*--------------------------------------------------------------------
		Get an object by index
	 
		index: The object index
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (when the object is bound to a specific document)
	 
		return: The requested object (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	std::unique_ptr<Obj> DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getObject(const ObjID& ID, std::optional<active::utility::String> tableID,
																			   std::optional<active::utility::String> documentID)  const {
		return getCache()->read(ID);
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getObject
	
	
	/*--------------------------------------------------------------------
		Get all objects
	 
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (filter for this document only - nullopt = all objects)
	 
		return: The requested objects (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	active::container::Vector<Obj> DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getObjects(std::optional<active::utility::String> tableID,
																						  std::optional<active::utility::String> documentID) const {
		return getCache()->read();
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getObjects
	
	
	/*--------------------------------------------------------------------
		Get all objects
	 
		filter: The object filter
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (filter for this document only - nullopt = all objects)
	 
		return: The requested objects (nullptr on failure)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	active::container::Vector<Obj> DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getObjects(const Filter& filter, std::optional<active::utility::String> tableID,
																						  std::optional<active::utility::String> documentID) const {
		return getCache()->read(filter);
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getObjects
	
	
	/*--------------------------------------------------------------------
		Write an object to the database
	 
		object: The object to write
		objID: The object ID
		objDocID: The object document-specific ID (unique within a specific document - nullopt if not document-bound)
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (when the object is bound to a specific document)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::write(const Obj& object, const ObjID& objID, std::optional<ObjID> objDocID,
														   utility::String::Option tableID, utility::String::Option documentID) const {
		getCache()->write(object);	//NB: In future we might support duplicating records if objID != obj.id
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::write
	
	
	/*--------------------------------------------------------------------
		Erase an object by index
	 
		objID: The object ID
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (when the object is bound to a specific document)
	 
		return: True if the object was successfully erased
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::erase(const ObjID& ID, std::optional<active::utility::String> tableID,
														   std::optional<active::utility::String> documentID) const {
		getCache()->erase(ID);
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::erase
	
	
	/*--------------------------------------------------------------------
		Erase all objects
	 
		tableID: Optional table ID (defaults to the first table)
		documentID: Optional document ID (filter for this document only - nullopt = all objects)
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::erase(std::optional<active::utility::String> tableID, std::optional<active::utility::String> documentID) const {
		getCache()->erase();
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::erase

	
	/*--------------------------------------------------------------------
		Get the database outline
	 
		return: The database outline
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::Outline DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getOutline() const {
		Outline result;
		if (getSchema().empty())
			return result;
			//NB: Currently implementing only a single table in a document store
		result.emplace_back(std::make_pair(getSchema()[0].ID, getCache()->keys()));
		return result;
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::getOutline
	
	
	/*--------------------------------------------------------------------
		Build new store data from the latest records
	 
		return: Data to store from the latest records
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	active::utility::Memory DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::buildStore() {
		active::utility::Memory result;
		Transport().send(std::forward<active::serialise::Cargo&&>(*getCache()), active::serialise::Identity{}, result);
		return result;
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::buildStore
	
	
	/*--------------------------------------------------------------------
		Merge existing stored data with incoming stored data (from an external source)
	 
		toMerge: The external stored data to merge
	 
		return: The merged data to be stored
	  --------------------------------------------------------------------*/
	template<typename Obj, typename ObjWrapper, typename Transport, typename ObjID>
	requires DocumentStorable<Obj, Transport>
	void DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::mergeStore(const active::utility::Memory& toMerge) {
			//Import the incoming records from the data to merge
		Cache incoming;
		if constexpr (std::is_same_v<ObjWrapper, Obj>)
			Transport().receive(std::forward<active::serialise::Cargo&&>(incoming), active::serialise::Identity{}, toMerge);
		else
			Transport().receive(ObjWrapper{incoming}, active::serialise::Identity{}, toMerge);
			//Get the data currently stored in the document
		getCache()->merge(std::forward<Cache&&>(incoming));
	} //DocumentStoreEngine<Obj, ObjWrapper, Transport, ObjID>::mergeStore
	
}

#endif	//SPECKLE_DATABASE_DOCUMENT_STORE_ENGINE
