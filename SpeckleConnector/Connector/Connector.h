#include "Speckle/Environment/Addon.h"
#include "Speckle/Utility/String.h"

namespace connector::database {
	
	class ModelCardDatabase;
	
}

namespace connector {
	
		///The Connector addon class
	class ConnectorAddon : public speckle::environment::Addon {
	public:
		
		// MARK: Functions (const)
		
		/*!
		 Get the model card database
		 @return The model card database
		 */
		const virtual database::ModelCardDatabase* getModelCards() const = 0;
		
	protected:
		/*!
		 Constructor
		 @param name The add-on name
		 */
		ConnectorAddon(const speckle::utility::String& name);
	};
	
	
	/*!
	 Get an object representing the connector instance
	 @return The active connector instance (nullptr if no connector is running)
	 */
	ConnectorAddon* connector();
	
}