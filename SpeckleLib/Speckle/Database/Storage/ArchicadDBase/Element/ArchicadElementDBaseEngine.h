#ifndef SPECKLE_DATABASE_ARCHICAD_ELEMENT_DBASE_ENGINE
#define SPECKLE_DATABASE_ARCHICAD_ELEMENT_DBASE_ENGINE

#include "Active/Database/Storage/DBaseEngine.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Storage/ArchicadDBase/ArchicadDBaseCore.h"
#include "Speckle/Database/Storage/Element/ElementStorage.h"
#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <algorithm>
#include <ranges>

namespace speckle::database {
	
	/*!
	 A database engine to read/write elements in an Archicad project database (local file or cloud-based)
	 */
	class ArchicadElementDBaseEngine : public ArchicadDBaseCore, public ElementStorage,
			public active::database::DBaseEngine<record::element::Element, BIMRecordID, BIMRecordID, BIMRecordID, ElementStorage::TableType>  {
	public:
		
		// MARK: - Types
		
		using base = active::database::DBaseEngine<record::element::Element, BIMRecordID, BIMRecordID, BIMRecordID>;
		using Element = record::element::Element;
		using Filter = ElementStorage::Filter;
		using Outline = base::Outline;
		using ObjIDList = base::ObjIDList;
		
		// MARK: - Constants
		
			///The memo table ID (supplementary element data)
		static const inline utility::Guid memoTable{utility::String{"fdff96d2-8c34-4f8b-8a76-a96a2b242758"}};
		
		// MARK: - Static functions
		
		/*!
		 Get the ID of the active Archicad table
		 @return The active table ID (nullopt on failure)
		 */
		static std::optional<BIMRecordID> getActiveTable();
				
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param id The document storage identifier
		 @param schema The document storage schema
		 */
		ArchicadElementDBaseEngine(const active::utility::NameID& id, ArchicadDBaseSchema&& schema) : ArchicadDBaseCore{id, std::move(schema)} {}
		ArchicadElementDBaseEngine(const ArchicadElementDBaseEngine&) = delete;
		
		// MARK: - Functions (const)
		
		/*!
		 Bring the view of this database to the front (i.e. so the user sees it)
		 @param tableID The ID of the table to bring to the front
		 */
		void bringViewToFront(BIMRecordID tableID) const;
		/*!
		 Get the current user element selection
		 @return A list of selected element IDs
		 */
		BIMLinkList getSelection() const;
		/*!
		 Set the element selection
		 */
		void setSelection(const BIMLinkList& elementIDs) const;
		/*!
		 Clear the element selection
		 */
		void clearSelection() const;
		/*!
		 Get the available dbase tables
		 @param targetType An optional filtr for table type/group to retrieve
		 @return A list of available tables
		 */
		TableIDList getTables(std::optional<TableType> targetType) const override;
		/*!
		 Find a filtered list of objects
		 @param filter The object filter (nullptr = find all objects)
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return A list containing IDs of found elements (empty if none found)
		 */
		virtual BIMRecordIDList findObjects(const Filter& filter = nullptr, std::optional<BIMRecordID> tableID = std::nullopt,
									  std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get an object by index
		 @param objID The object ID
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested object (nullptr on failure)
		 */
		std::unique_ptr<Element> getObject(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get an object in a transportable form, e.g. packaged for serialisation
		 @param objID The object ID
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested wrapped cargo (nullptr on failure)
		 */
		active::serialise::Cargo::Unique getObjectCargo(const BIMRecordID& objID, std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get all objects
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The requested objects (nullptr on failure)
		 */
		active::container::Vector<Element> getObjects(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Get a filtered list of objects
		 @param filter The object filter
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return The filtered objects (nullptr on failure)
		 */
		active::container::Vector<Element> getObjects(const Filter& filter, std::optional<BIMRecordID> tableID = std::nullopt,
												  std::optional<BIMRecordID> documentID = std::nullopt) const override;
		/*!
		 Write an object to the database
		 @param object The object to write
		 @param objID The object ID
		 @param objDocID The object document-specific ID (unique within a specific document - nullopt if not document-bound)
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 */
		void write(const Element& object, const BIMRecordID& objID, std::optional<BIMRecordID> objDocID = std::nullopt,
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
				
	private:
		void setTable(std::optional<BIMRecordID> tableID = std::nullopt, std::optional<BIMRecordID> documentID = std::nullopt);
	};
	
}

#endif	//SPECKLE_DATABASE_ARCHICAD_ELEMENT_DBASE_ENGINE
