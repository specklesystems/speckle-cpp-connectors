#ifndef SPECKLE_ENVIRONMENT_PROJECT
#define SPECKLE_ENVIRONMENT_PROJECT

#include "Active/File/Path.h"
#include "Speckle/Utility/String.h"

namespace speckle::database {
	class BIMElementDatabase;
}

namespace speckle::environment {
	
	class Addon;
	
	/*!
	 A BIM project
	 
	 This class is currently skeletal, but is intended to be the primnary conduit for any document-based data, e.g. elements, attributes, properties
	 etc. Any databases managing document-based content should be retrieved from this class rather than Add-on (or subclasses) or static functions.
	*/
	class Project {
	public:

		// MARK: - Types
		
			///Shared pointer
		using Shared = std::shared_ptr<Project>;
			///Weak pointer
		using Weak = std::weak_ptr<Project>;
			///Project information
		struct Info {
				//The project name
			utility::String name;
				//The project ID
			utility::String ID;
				//Either local path or server URL where the project is stored (nullopt if memory-based only, i.e. unsaved)
			active::file::Path::Option path;
				//True if the project is shared (cloud-based)
			bool isShared = false;
		};

		// MARK: - Constructors
		
		Project(const Project&) = delete;
		/*!
		 Destructor
		 */
		~Project();
		
		// MARK: - Functions (const)
		
		/*!
		 Get information about the project
		 @return Project information
		 */
		Info getInfo() const;
		/*!
		 Get the account database
		 @return The account database
		 */
		const database::BIMElementDatabase* getElementDatabase() const { return m_element.get(); }
		
		// MARK: - Functions (mutating)
		
	protected:
		friend class speckle::environment::Addon;
		
		/*!
		 Default constructor
		 
		 NB: Only the Addon class can create projects. Clients can get the active project from the running add-on.
		 */
		Project();
		
	private:
		std::unique_ptr<database::BIMElementDatabase> m_element;
	};

}

#endif	//SPECKLE_ENVIRONMENT_PROJECT
