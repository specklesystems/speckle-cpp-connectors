#ifndef CONNECTOR_DATABASE_BIM_ATTRIBUTE_DATABASE
#define CONNECTOR_DATABASE_BIM_ATTRIBUTE_DATABASE

#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Attribute/Attribute.h"
#include "Speckle/Utility/Guid.h"

namespace active::event {
	class Subscriber;
}

namespace speckle::database {
	
	/*!
	 Database of model attributes relating to a specific project
	 */
	class BIMAttributeDatabase {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		BIMAttributeDatabase();
		BIMAttributeDatabase(const BIMAttributeDatabase&) = delete;
		/*!
		 Destructor
		 */
		~BIMAttributeDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get a specified attribute
		 @param attributeID The ID of the target attribute
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested attribute (nullptr on failure)
		 */
		record::attribute::Attribute::Unique getAttribute(const BIMRecordID& attributeID, std::optional<BIMRecordID> tableID = std::nullopt,
													std::optional<BIMRecordID> documentID = std::nullopt) const;
		/*!
		 Get a specified attribute
		 @param link A link to the target attribute
		 @return The requested attribute (nullptr on failure)
		 */
		record::attribute::Attribute::Unique getAttribute(const BIMLink& link) const { return getAttribute(link, link.tableID, link.docID); }
		/*!
		 Get all model attributes
		 @return All the attributes
		 */
		active::container::Vector<record::attribute::Attribute> getAttributes() const;
		/*!
		 Write an attribute to storage
		 @param attribute The attribute to write
		 */
		void write(const record::attribute::Attribute& attribute) const;
		/*!
		 Erase an attribute
		 @param attributeID The ID of the attribute to erase
		 */
		void erase(const speckle::utility::Guid& attributeID) const;
		
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
		/*!
		 Get the ID of a storey from a specified index
		 @param index The storey index
		 @return The storey ID (nullopt on failure)
		 */
		std::optional<BIMRecordID> getStoreyID(short index) const;
#endif
		
	private:
		class Engine;
		class Store;
			///Model attribute database storage
		std::shared_ptr<Engine> m_engine;
		std::shared_ptr<Store> m_store;
	};

}

#endif	//CONNECTOR_DATABASE_BIM_ATTRIBUTE_DATABASE
