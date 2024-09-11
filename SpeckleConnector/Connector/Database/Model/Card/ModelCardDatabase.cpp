#include "Connector/Database/Model/Card/ModelCardDatabase.h"

#include "Active/Database/Storage/Storage.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Speckle/Database/Storage/DocumentStore/DocumentStoreEngine.h"

#include <array>

using namespace active::container;
using namespace active::database;
using namespace active::serialise;
using namespace active::serialise::json;
using namespace active::setting;
using namespace connector::database;
using namespace speckle::database;
using namespace speckle::utility;

namespace connector::database {

		///ModelCard database engine declaration
	using ModelCardEngine = DocumentStoreEngine<ModelCard, JSONTransport, active::utility::String>;

		///ModelCard database storage declaration
	class ModelCardDatabase::Store : public active::database::Storage<connector::database::ModelCard, active::serialise::json::JSONTransport,
			active::utility::String, active::utility::String, active::utility::String, active::utility::String> {
		using base = active::database::Storage<connector::database::ModelCard, active::serialise::json::JSONTransport,
			active::utility::String, active::utility::String, active::utility::String, active::utility::String>;
		using base::base;
	};
	
}

namespace {
	
		///The document storage identifier for model cards
	const char* modelCardDBaseName = "connector::database::ModelModelCardDatabase";
		///The document storage identifier for model cards
	const char* modelCardTableName = "models";

}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
ModelCardDatabase::ModelCardDatabase() {
	
	auto engine = std::make_shared<ModelCardEngine>(modelCardDBaseName,
			//Schema
		 DBaseSchema{active::utility::String{modelCardDBaseName},
				//Tables
			{
					//Model card table
				{
					modelCardTableName, 0, 0, {}	//The table specifics aren't relevant in this context
				}
			}
		}
	);
	m_store = std::make_unique<Store>(std::move(engine));
} //ModelCardDatabase::ModelCardDatabase


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ModelCardDatabase::~ModelCardDatabase() {}


/*--------------------------------------------------------------------
	Get all model cards
 
	return: All the cards
  --------------------------------------------------------------------*/
Vector<ModelCard> ModelCardDatabase::getCards() const {
	return m_store->getObjects();
} //ModelCardDatabase::getCards


/*--------------------------------------------------------------------
	Get a serialisation wrapper for the database
 
	return: A database wrapper
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> ModelCardDatabase::wrapper() const {
	return m_store->wrapper();
} //ModelCardDatabase::wrapper
