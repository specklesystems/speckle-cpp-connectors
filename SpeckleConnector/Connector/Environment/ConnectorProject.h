#ifndef CONNECTOR_ENVIRONMENT_CONNECTOR_PROJECT
#define CONNECTOR_ENVIRONMENT_CONNECTOR_PROJECT

#include "Speckle/Environment/Project.h"

namespace connector::database {
	class ModelCardDatabase;
}

namespace connector::environment {
	
	/*!
	 A BIM project contained Speckle connector data, e.g. a model card database
	*/
	class ConnectorProject : public speckle::environment::Project {
	public:

		// MARK: - Types
		
			///Shared pointer
		using Shared = std::shared_ptr<ConnectorProject>;
			///Weak pointer
		using Weak = std::weak_ptr<ConnectorProject>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ConnectorProject();
		
		ConnectorProject(const ConnectorProject&) = delete;
		/*!
		 Destructor
		 */
		~ConnectorProject();
		
		// MARK: - Functions (const)
		
		/*!
		 Get the model card database
		 @return The model card database
		 */
		const connector::database::ModelCardDatabase* getModelCardDatabase() const;
		
	private:
			///The model card database
		std::unique_ptr<connector::database::ModelCardDatabase> m_modelCards;
	};

}

#endif	//CONNECTOR_ENVIRONMENT_CONNECTOR_PROJECT
