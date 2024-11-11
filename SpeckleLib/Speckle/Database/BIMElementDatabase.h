#ifndef CONNECTOR_DATABASE_BIM_ELEMENT_DATABASE
#define CONNECTOR_DATABASE_BIM_ELEMENT_DATABASE

#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Database/Storage/Element/ElementStorage.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Record/Element/Interface/Part.h"
#include "Speckle/Utility/Guid.h"

namespace active::event {
	class Subscriber;
}

namespace speckle::record::element {
	class Memo;
}

namespace speckle::database {
	
	/*!
	 Database of model elements relating to a specific project
	 */
	class BIMElementDatabase : public ElementStorage {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		BIMElementDatabase();
		BIMElementDatabase(const BIMElementDatabase&) = delete;
		/*!
		 Destructor
		 */
		~BIMElementDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get the available element tables
		 @param targetType An optional filtr for table type to retrieve, e.g. get all sections (nullopt = all table types)
		 @return A set of available tables
		 */
		BIMRecordIDList getTables(std::optional<TableType> targetType) const;
		/*!
		 Get the default dbase table
		 @return The default dbase table (nullopt if no table is available)
		 */
		std::optional<BIMRecordID> getDefaultTable() const;
		/*!
		 Set the default dbase table
		 @param tableID The new default dbase table
		 */
		void setDefaultTable(const BIMRecordID& tableID) const;
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
		 Find a filtered list of objects
		 @param filter The object filter (nullptr = find all objects)
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return A list containing IDs of found elements (empty if none found)
		 */
		BIMRecordIDList findElements(const Filter& filter = nullptr, std::optional<BIMRecordID> tableID = std::nullopt,
											  std::optional<BIMRecordID> documentID = std::nullopt) const;
		/*!
		 Get a specified element
		 @param elementID The ID of the target element
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested element (nullptr on failure)
		 */
		std::unique_ptr<record::element::Element> getElement(const BIMRecordID& elementID, std::optional<BIMRecordID> tableID = std::nullopt,
													std::optional<BIMRecordID> documentID = std::nullopt) const;
		/*!
		 Get a specified element
		 @param link A link to the target element
		 @return The requested element (nullptr on failure)
		 */
		std::unique_ptr<record::element::Element> getElement(const BIMLink& link) const { return getElement(link, link.tableID, link.docID); }
		/*!
		 Get all model elements
		 @param tableID Optional table ID (defaults to the first table)
		 @param documentID Optional document ID (filter for this document only - nullopt = all objects)
		 @return All the elements
		 */
		active::container::Vector<record::element::Element> getElements(std::optional<BIMRecordID> tableID = std::nullopt,
																		std::optional<BIMRecordID> documentID = std::nullopt) const;
		/*!
		 Get memo memo (supplementary) data for a specified element
		 @param elementID The of the source element
		 @param filter Filter for the required supplementary data
		 @return The requested element memo data (nullptr on failure)
		 */
		std::unique_ptr<record::element::Memo> getMemo(const BIMRecordID& elementID, record::element::Part::filter_bits filter) const;
		/*!
		 Write an element to storage
		 @param element The element to write
		 */
		void write(const record::element::Element& element) const;
		/*!
		 Erase an element
		 @param elementID The ID of the element to erase
		 */
		void erase(const speckle::utility::Guid& elementID) const;
		
	private:
		class Engine;
		class Store;
			///Model element database storage
		std::shared_ptr<Engine> m_engine;
		std::shared_ptr<Store> m_store;
	};

}

#endif	//CONNECTOR_DATABASE_BIM_ELEMENT_DATABASE
