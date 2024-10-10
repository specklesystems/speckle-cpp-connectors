#include "Speckle/Database/BIMAttributeDatabase.h"

#include "Active/Database/Storage/Storage.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Database/Storage/ArchicadDBase/Attribute/ArchicadAttributeDBaseEngine.h"
#include "Speckle/Record/Attribute/Attribute.h"

#include <array>

using namespace active::container;
using namespace active::database;
using namespace active::event;
using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record;
using namespace speckle::record::attribute;
using namespace speckle::database;
using namespace speckle::utility;

namespace speckle::database {
	
	///Define other platform engines here as required
#ifdef ARCHICAD
	using AttributeDatabaseEngine = ArchicadAttributeDBaseEngine;
#endif

		///Attribute database engine declaration
	class BIMAttributeDatabase::Engine : public AttributeDatabaseEngine {
		using base = ArchicadAttributeDBaseEngine;
		using base::base;
	};

		///Attribute database storage declaration
	class BIMAttributeDatabase::Store : public Storage<Attribute, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID> {
		using base = Storage<Attribute, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID>;
		using base::base;
	};
	
}

namespace {
	
		///The database storage identifier for attributes
	const char* attributeDBaseName = "speckle::database::BIMAttributeDatabase";
		///The primary model table, e.g. floor plan in Archicad
	const char* modelTableName = "Model";

}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
BIMAttributeDatabase::BIMAttributeDatabase() {
	m_engine = std::make_shared<Engine>(attributeDBaseName,
			//Schema
		 DBaseSchema{active::utility::String{attributeDBaseName},
				//Tables
			{
					//Model attribute table
				{
					modelTableName, 0, 0, {}	//The primary model. Additonal tables could be linked to other drawings/layouts in future
				}
			}
		}
	);
	m_store = std::make_shared<Store>(m_engine);
} //BIMAttributeDatabase::BIMAttributeDatabase


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BIMAttributeDatabase::~BIMAttributeDatabase() {}


/*--------------------------------------------------------------------
	Get a specified attribute
 
	attributeID: The ID of the target attribute
 
	return: The requested attribute (nullptr on failure)
  --------------------------------------------------------------------*/
Attribute::Unique BIMAttributeDatabase::getAttribute(const BIMRecordID& attributeID, std::optional<BIMRecordID> tableID,
											   std::optional<BIMRecordID> documentID) const {
	return m_engine->getObject(attributeID, tableID, documentID);
} //BIMAttributeDatabase::getAttribute


/*--------------------------------------------------------------------
	Get all attributes
 
	return: All the attributes
  --------------------------------------------------------------------*/
Vector<Attribute> BIMAttributeDatabase::getAttributes() const {
	return m_store->getObjects();
} //BIMAttributeDatabase::getAttributes


/*--------------------------------------------------------------------
	Write an attribute to storage
 
	attribute: The attribute to write
  --------------------------------------------------------------------*/
void BIMAttributeDatabase::write(const Attribute& attribute) const {
	m_store->write(attribute);
} //BIMAttributeDatabase::write


/*--------------------------------------------------------------------
	Erase an attribute
 
	attributeID: The ID of the attribute to erase
  --------------------------------------------------------------------*/
void BIMAttributeDatabase::erase(const Guid& attributeID) const {
	m_store->erase(attributeID);
} //BIMAttributeDatabase::erase

		
#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Get attribute data direct from the AC API. For internal use - avoid direct use
 
	link: A link to the required attribute
 
	return: The AC API attribute data
  --------------------------------------------------------------------*/
std::optional<API_Attribute> BIMAttributeDatabase::getAPIData(const BIMLink& link) const {
	return m_engine->getAPIData(link);
} //BIMAttributeDatabase::getAPIData


/*--------------------------------------------------------------------
	Get storey data direct from the AC API. For internal use - avoid direct use
 
	link: A link to the required storey
 
	return: The AC API storey data
  --------------------------------------------------------------------*/
std::optional<API_StoryType> BIMAttributeDatabase::getAPIStorey(const BIMLink& link) const {
	return m_engine->getAPIStorey(link);
} //BIMAttributeDatabase::getAPIData
#endif
