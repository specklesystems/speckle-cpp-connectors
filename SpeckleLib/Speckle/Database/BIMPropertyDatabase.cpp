#include "Speckle/Database/BIMPropertyDatabase.h"

#include "Active/Database/Storage/Storage.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Database/Storage/ArchicadDBase/Property/ArchicadPropertyDBaseEngine.h"
#include "Speckle/Record/Property/Setting.h"

#include <array>

using namespace active::container;
using namespace active::database;
using namespace active::event;
using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record;
using namespace speckle::record::property;
using namespace speckle::database;
using namespace speckle::utility;

namespace speckle::database {
	
	///Define other platform engines here as required
#ifdef ARCHICAD
	using PropertyDatabaseEngine = ArchicadPropertyDBaseEngine;
#endif

		///Property database engine declaration
	class BIMPropertyDatabase::Engine : public PropertyDatabaseEngine {
		using base = ArchicadPropertyDBaseEngine;
		using base::base;
	};

		///Property database storage declaration
	class BIMPropertyDatabase::Store : public Storage<Template, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID> {
		using base = Storage<Template, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID>;
		using base::base;
	};
	
}

namespace {
	
		///The database storage identifier for properties
	const char* propertyDBaseName = "speckle::database::BIMPropertyDatabase";
		///The primary properties table
	const char* propertyTableName = "Properties";

}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
BIMPropertyDatabase::BIMPropertyDatabase() {
	m_engine = std::make_shared<Engine>(propertyDBaseName,
			//Schema
		 DBaseSchema{active::utility::String{propertyDBaseName},
				//Tables
			{
					//Model property table
				{
					propertyTableName, 0, 0, {}
				}
			}
		}
	);
	m_store = std::make_shared<Store>(m_engine);
} //BIMPropertyDatabase::BIMPropertyDatabase


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BIMPropertyDatabase::~BIMPropertyDatabase() {}


/*--------------------------------------------------------------------
	Get a specified property
 
	propertyID: The ID of the target property
 
	return: The requested property (nullptr on failure)
  --------------------------------------------------------------------*/
Template::Unique BIMPropertyDatabase::getProperty(const BIMRecordID& propertyID, std::optional<BIMRecordID> tableID,
											   std::optional<BIMRecordID> documentID) const {
	return m_engine->getObject(propertyID, tableID, documentID);
} //BIMPropertyDatabase::getProperty


/*--------------------------------------------------------------------
	Get a specified property
 
	link: A link to the target property
 
	return: The requested property (nullptr on failure)
  --------------------------------------------------------------------*/
Template::Unique BIMPropertyDatabase::getProperty(const BIMLink& link) const {
	return getProperty(link, link.tableID, link.docID);
} //BIMPropertyDatabase::getProperty


/*--------------------------------------------------------------------
	Get all properties
 
	return: All the properties
  --------------------------------------------------------------------*/
Vector<Template> BIMPropertyDatabase::getProperties() const {
	return m_store->getObjects();
} //BIMPropertyDatabase::getPropertys


/*--------------------------------------------------------------------
	Find all property templates linked to specified classifications
 
	classifications: The classifications
 
	return: A list of shared pointers to linked property templates
  --------------------------------------------------------------------*/
std::vector<std::shared_ptr<Template>> BIMPropertyDatabase::findTemplatesByClassification(const BIMRecordIDList& classifications) const {
	return m_engine->findTemplatesByClassification(classifications);
} //BIMPropertyDatabase::findTemplatesByClassification


/*--------------------------------------------------------------------
	Write an property to storage
 
	property: The property to write
  --------------------------------------------------------------------*/
void BIMPropertyDatabase::write(const Template& property) const {
	m_store->write(property);
} //BIMPropertyDatabase::write


/*--------------------------------------------------------------------
	Erase an property
 
	propertyID: The ID of the property to erase
  --------------------------------------------------------------------*/
void BIMPropertyDatabase::erase(const Guid& propertyID) const {
	m_store->erase(propertyID);
} //BIMPropertyDatabase::erase
