#ifndef CONNECTOR_DATABASE_BIM_PROPERTY_DATABASE
#define CONNECTOR_DATABASE_BIM_PROPERTY_DATABASE

#include "Speckle/Database/Identity/BIMLink.h"
#include "Speckle/Record/Property/Template.h"
#include "Speckle/Utility/Guid.h"

namespace active::event {
	class Subscriber;
}

namespace speckle::database {
	
	/*!
	 Database of property templates relating to a specific project
	 
	 Note that this database manages just the property templates, not the values. Property values are attached to elements
	 */
	class BIMPropertyDatabase {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		BIMPropertyDatabase();
		BIMPropertyDatabase(const BIMPropertyDatabase&) = delete;
		/*!
		 Destructor
		 */
		~BIMPropertyDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get a specified property
		 @param propertyID The ID of the target property
		 @param tableID Optional table ID (defaults to the floor plan)
		 @param documentID Optional document ID (when the object is bound to a specific document)
		 @return The requested property (nullptr on failure)
		 */
		record::property::Template::Unique getProperty(const BIMRecordID& propertyID, std::optional<BIMRecordID> tableID = std::nullopt,
													   std::optional<BIMRecordID> documentID = std::nullopt) const;
		/*!
		 Get a specified property
		 @param link A link to the target property
		 @return The requested property (nullptr on failure)
		 */
		record::property::Template::Unique getProperty(const BIMLink& link) const;
		/*!
		 Get all model properties
		 @return All the properties
		 */
		active::container::Vector<record::property::Template> getProperties() const;
		/*!
		 Find all property templates linked to specified classifications
		 @param classifications The classifications
		 @return A list of shared pointers to linked property templates
		 */
		std::vector<std::shared_ptr<record::property::Template>> findTemplatesByClassification(const BIMRecordIDList& classifications) const;
		/*!
		 Write an property to storage
		 @param property The property to write
		 */
		void write(const record::property::Template& property) const;
		/*!
		 Erase an property
		 @param propertyID The ID of the property to erase
		 */
		void erase(const speckle::utility::Guid& propertyID) const;
		
	private:
		class Engine;
		class Store;
			///Model property database storage
		std::shared_ptr<Engine> m_engine;
		std::shared_ptr<Store> m_store;
	};

}

#endif	//CONNECTOR_DATABASE_BIM_PROPERTY_DATABASE
