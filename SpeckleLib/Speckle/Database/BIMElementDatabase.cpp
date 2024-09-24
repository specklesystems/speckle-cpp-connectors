#include "Speckle/Database/BIMElementDatabase.h"

#include "Active/Database/Storage/Storage.h"
#include "Active/Serialise/UnboxedTransport.h"
#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Database/Storage/ArchicadDBase/Element/ArchicadElementDBaseEngine.h"
#include "Speckle/Record/Element/Element.h"

#include <array>

using namespace active::container;
using namespace active::database;
using namespace active::event;
using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::record;
using namespace speckle::record::element;
using namespace speckle::database;
using namespace speckle::utility;

namespace speckle::database {
	
	///Define other platform engines here as required
#ifdef ARCHICAD
	using ElementDatabaseEngine = ArchicadElementDBaseEngine;
#endif

		///Element database engine declaration
	class BIMElementDatabase::Engine : public ElementDatabaseEngine {
		using base = ArchicadElementDBaseEngine;
		using base::base;
	};

		///Element database storage declaration
	class BIMElementDatabase::Store : public Storage<Element, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID> {
		using base = Storage<Element, UnboxedTransport, BIMRecordID, BIMRecordID, BIMRecordID, BIMRecordID>;
		using base::base;
	};
	
}

namespace {
	
		///The database storage identifier for elements
	const char* elementDBaseName = "speckle::database::BIMElementDatabase";
		///The primary model table, e.g. floor plan in Archicad
	const char* modelTableName = "Model";

}

/*--------------------------------------------------------------------
	Constructor
  --------------------------------------------------------------------*/
BIMElementDatabase::BIMElementDatabase() {
	m_engine = std::make_shared<Engine>(elementDBaseName,
			//Schema
		 DBaseSchema{active::utility::String{elementDBaseName},
				//Tables
			{
					//Model element table
				{
					modelTableName, 0, 0, {}	//The primary model. Additonal tables could be linked to other drawings/layouts in future
				}
			}
		}
	);
	m_store = std::make_shared<Store>(m_engine);
} //BIMElementDatabase::BIMElementDatabase


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
BIMElementDatabase::~BIMElementDatabase() {}


/*--------------------------------------------------------------------
	Get all elements
 
	return: All the elements
  --------------------------------------------------------------------*/
Vector<Element> BIMElementDatabase::getElements() const {
	return m_store->getObjects();
} //BIMElementDatabase::getElements


/*--------------------------------------------------------------------
	Write an element to storage
 
	element: The element to write
  --------------------------------------------------------------------*/
void BIMElementDatabase::write(const Element& element) const {
	m_store->write(element);
} //BIMElementDatabase::write


/*--------------------------------------------------------------------
	Erase an element
 
	elementID: The ID of the element to erase
  --------------------------------------------------------------------*/
void BIMElementDatabase::erase(const Guid& elementID) const {
	m_store->erase(elementID);
} //BIMElementDatabase::erase
