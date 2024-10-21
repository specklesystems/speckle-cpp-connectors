#ifndef SPECKLE_DATABASE_ARCHICAD_PROPERTY_DBASE_ENGINE
#define SPECKLE_DATABASE_ARCHICAD_PROPERTY_DBASE_ENGINE

#include "Active/Database/Storage/DBaseEngine.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Storage/ArchicadDBase/ArchicadDBaseCore.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Property/Template.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <algorithm>
#include <ranges>

namespace speckle::database {
	
	/*!
	 A database engine to read/write property templates in an Archicad project database (local file or cloud-based)
	 
	 Property templates describe the characteristics and metadate attached to element property values. As such the templates may be shared
	 amongst any number of Property objects
	 */
	class ArchicadPropertyDBaseEngine : public ArchicadDBaseCore,
			public active::database::DBaseEngine<record::property::Template, BIMRecordID, BIMRecordID, BIMRecordID>  {
	public:
		
		// MARK: - Types
		
		using base = active::database::DBaseEngine<record::property::Template, BIMRecordID, BIMRecordID, BIMRecordID>;
		using Template = record::property::Template;
		using Filter = base::Filter;
		using Outline = base::Outline;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param id The document storage identifier
		 @param schema The document storage schema
		 */
		ArchicadPropertyDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema);
		ArchicadPropertyDBaseEngine(const ArchicadPropertyDBaseEngine&) = delete;
		/*!
		 Destructor
		 */
		~ArchicadPropertyDBaseEngine();
		
		// MARK: - Functions (const)
		
		/*!
		 Find all property templates linked to specified classifications
		 @param classifications The classifications
		 @return A list of shared pointers to linked property templates
		 */
		std::vector<std::shared_ptr<Template>> findTemplatesByClassification(const BIMRecordIDList& classifications) const;
		/*!
		 Get an object by ID
		 @param objID The object ID
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested object (nullptr on failure)
		 */
		std::unique_ptr<Template> getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get an object in a transportable form, e.g. packaged for serialisation
		 @param objID The object ID
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return: The requested wrapped cargo (nullptr on failure)
		 */
		active::serialise::Cargo::Unique getObjectCargo(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get all objects
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The requested objects (nullptr on failure)
		 */
		active::container::Vector<Template> getObjects(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get a filtered list of objects
		 @param filter The object filter
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The filtered objects (nullptr on failure)
		 */
		active::container::Vector<Template> getObjects(const Filter& filter, std::optional<BIMRecordID> tableID = std::nullopt,
												  std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Write an object to the database
		 @param object The object to write
		 @param objID The object ID
		 @param objDocID The object document-specific ID (unique within a specific document - nullopt if not document-bound)
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 */
		void write(const Template& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID = std::nullopt,
				   std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Erase an object by index
		 @param ID The object ID
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @throw Exception thrown on SQL error
		 */
		void erase(const BIMRecordID& ID, std::optional<BIMRecordID> tableID = std::nullopt,
				   std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Erase all objects
		 @param tableID Optional table ID (defaults to the floor plan)
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

#endif	//SPECKLE_DATABASE_ARCHICAD_PROPERTY_DBASE_ENGINE
