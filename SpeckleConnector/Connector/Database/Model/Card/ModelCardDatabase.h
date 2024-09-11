#ifndef CONNECTOR_DATABASE_MODEL_CARD_DATABASE
#define CONNECTOR_DATABASE_MODEL_CARD_DATABASE

#include "Connector/Database/Model/Card/ModelCard.h"

namespace active::event {
	class Subscriber;
}

namespace connector::database {
	
	/*!
	 Database of model cards relating to a specific project
	 */
	class ModelCardDatabase {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		ModelCardDatabase();
		ModelCardDatabase(const ModelCardDatabase&) = delete;
		/*!
		 Destructor
		 */
		~ModelCardDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get all model cards
		 @return All the cards
		 */
		active::container::Vector<ModelCard> getCards() const;
		/*!
		 Get a serialisation wrapper for the database
		 @return A database wrapper
		 */
		std::unique_ptr<active::serialise::Cargo> wrapper() const;
		/*!
		 Get the database subscription (the content is document-based, and must react to document operations)
		 @return The database subscription (add weakly to publisher)
		 */
		std::shared_ptr<active::event::Subscriber> getSubscription() const;
		
		// MARK: - Functions (mutating)
				
	private:
		class Store;
			///Model card database storage
		std::shared_ptr<Store> m_store;
	};

}

#endif	//CONNECTOR_DATABASE_MODEL_CARD_DATABASE
