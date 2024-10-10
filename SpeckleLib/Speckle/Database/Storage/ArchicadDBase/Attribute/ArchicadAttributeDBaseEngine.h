#ifndef SPECKLE_DATABASE_ARCHICAD_ATTRIBUTE_DBASE_ENGINE
#define SPECKLE_DATABASE_ARCHICAD_ATTRIBUTE_DBASE_ENGINE

#include "Active/Database/Storage/DBaseEngine.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Storage/ArchicadDBase/ArchicadDBaseCore.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Attribute/Attribute.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <algorithm>
#include <ranges>

namespace speckle::database {
	
	/*!
	 A database engine to read/write elements in an Archicad project database (local file or cloud-based)
	 
	 For attribute indices:
	 - Each attribute type is considered to be stored in a dedicated table
	 - For Archicad:
	 	- The table ID is typically the attribute type ID
	 	- The record ID is the attribute index
	 	- Storeys are also treated as an attribute (the API treats them separately, although application to elements is virtually the identical)
	 */
	class ArchicadAttributeDBaseEngine : public ArchicadDBaseCore,
			public active::database::DBaseEngine<record::attribute::Attribute, BIMRecordID, BIMRecordID, BIMRecordID>  {
	public:
		
		// MARK: - Types
		
		using base = active::database::DBaseEngine<record::attribute::Attribute, BIMRecordID, BIMRecordID, BIMRecordID>;
		using Attribute = record::attribute::Attribute;
		using Filter = base::Filter;
		using Outline = base::Outline;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param id The document storage identifier
		 */
		ArchicadAttributeDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema);
		ArchicadAttributeDBaseEngine(const ArchicadAttributeDBaseEngine&) = delete;
		/*!
		 Destructor
		 */
		~ArchicadAttributeDBaseEngine();
		
		// MARK: - Functions (const)
		
		/*!
		 Get an object by ID
		 @param objID The object ID
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested object (nullptr on failure)
		 */
		std::unique_ptr<Attribute> getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
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
		active::container::Vector<Attribute> getObjects(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get a filtered list of objects
		 @param filter The object filter
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The filtered objects (nullptr on failure)
		 */
		active::container::Vector<Attribute> getObjects(const Filter& filter, std::optional<BIMRecordID> tableID = std::nullopt,
												  std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Write an object to the database
		 @param object The object to write
		 @param objID The object ID
		 @param objDocID The object document-specific ID (unique within a specific document - nullopt if not document-bound)
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 */
		void write(const Attribute& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID = std::nullopt,
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
				
#ifdef ARCHICAD
		/*!
		 Get attribute data direct from the AC API. For internal use - avoid direct use
		 @param link A link to the required attribute
		 @return The AC API attribute data
		 */
		std::optional<API_Attribute> getAPIData(const BIMLink& link) const;
		/*!
		 Get storey data direct from the AC API. For internal use - avoid direct use
		 @param link A link to the required storey
		 @return The AC API storey data
		 */
		std::optional<API_StoryType> getAPIStorey(const BIMLink& link) const;
#endif

	private:
		void setTable(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt);

		class StoreyCache;
			///Cache of storeys in the database (saves repeated lookups)
		mutable std::unique_ptr<StoreyCache> m_storeyCache;
	};
	
}

#endif	//SPECKLE_DATABASE_ARCHICAD_ATTRIBUTE_DBASE_ENGINE
