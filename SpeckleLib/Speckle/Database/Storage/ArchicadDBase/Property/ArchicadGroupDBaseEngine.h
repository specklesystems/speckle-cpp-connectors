#ifndef SPECKLE_DATABASE_ARCHICAD_GROUP_DBASE_ENGINE
#define SPECKLE_DATABASE_ARCHICAD_GROUP_DBASE_ENGINE

#include "Active/Database/Storage/DBaseEngine.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Storage/ArchicadDBase/ArchicadDBaseCore.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Property/Group.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <algorithm>
#include <ranges>

namespace speckle::database {
	
	/*!
	 A database engine to read/write property groups in an Archicad project database (local file or cloud-based)
	 
	 Property groups can be attached to property templates to support collections of types linked to a specific role
	 */
	class ArchicadGroupDBaseEngine : public ArchicadDBaseCore,
			public active::database::DBaseEngine<record::property::Group, BIMRecordID, BIMRecordID, BIMRecordID>  {
	public:
		
		// MARK: - Types
		
		using base = active::database::DBaseEngine<record::property::Group, BIMRecordID, BIMRecordID, BIMRecordID>;
		using Group = record::property::Group;
		using Filter = base::Filter;
		using Outline = base::Outline;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param id The document storage identifier
		 @param schema The document storage schema
		 */
		ArchicadGroupDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema);
		ArchicadGroupDBaseEngine(const ArchicadGroupDBaseEngine&) = delete;
		/*!
		 Destructor
		 */
		~ArchicadGroupDBaseEngine();
		
		// MARK: - Functions (const)
		
		/*!
		 Get an object by ID
		 @param objID The object ID
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested object (nullptr on failure)
		 */
		std::unique_ptr<Group> getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get an object in a transportable form, e.g. packaged for serialisation
		 @param objID The object ID
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return: The requested wrapped cargo (nullptr on failure)
		 */
		active::serialise::Cargo::Unique getObjectCargo(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get all objects
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The requested objects (nullptr on failure)
		 */
		active::container::Vector<Group> getObjects(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get a filtered list of objects
		 @param filter The object filter
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The filtered objects (nullptr on failure)
		 */
		active::container::Vector<Group> getObjects(const Filter& filter, std::optional<BIMRecordID> tableID = std::nullopt,
												  std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Write an object to the database
		 @param object The object to write
		 @param objID The object ID
		 @param objDocID The object document-specific ID (unique within a specific document - nullopt if not document-bound)
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 */
		void write(const Group& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID = std::nullopt,
				   std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Erase an object by index
		 @param ID The object ID
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(const BIMRecordID& ID, std::optional<BIMRecordID> tableID = std::nullopt,
				   std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Erase all objects
		 @param tableID Optional table ID (default selected based on record type)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get the database outline
		 @return The database outline
		 */
		Outline getOutline() const override;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Handle a project event
		 @param event The project event
		 @return True if the event should be closed
		 */
		bool handle(const event::ProjectEvent& event) override;
				
	private:
		/*!
		 Ensure the cache is current
		 @return True if the cache contains valid te templates
		 */
		bool validateCache() const;
				
			//Cached templates
		class Cache;
		mutable std::unique_ptr<Cache> m_cache;
	};
	
}

#endif	//SPECKLE_DATABASE_ARCHICAD_GROUP_DBASE_ENGINE
