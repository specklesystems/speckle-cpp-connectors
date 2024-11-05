#include "Connector/Environment/ConnectorProject.h"

#include "Connector/Connector.h"
#include "Connector/Database/ModelCardDatabase.h"

using namespace connector::database;
using namespace connector::environment;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ConnectorProject::ConnectorProject() {
	m_modelCards = std::make_unique<ModelCardDatabase>();
}


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ConnectorProject::~ConnectorProject() {}


/*--------------------------------------------------------------------
	Get the model card database
 
	return; The model card database
  --------------------------------------------------------------------*/
const ModelCardDatabase* ConnectorProject::getModelCardDatabase() const {
	return m_modelCards.get();
} //ConnectorProject::getModelCardDatabase
