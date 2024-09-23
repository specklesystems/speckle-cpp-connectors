#ifndef SPECKLE_ENVIRONMENT_ADDON
#define SPECKLE_ENVIRONMENT_ADDON

#include "Active/Environment/App.h"
#include "Speckle/Utility/String.h"

namespace speckle::environment {
	
	class Project;
	
	/*!
		A base class for an addon
	*/
	class Addon : public active::environment::App {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor (NB: this object is assumed to be the active instance)
		 @param identity Optional name/ID for the subscriber
		 */
		Addon(const active::utility::NameID& identity);
		Addon(const App&) = delete;
		/*!
		 Destructor
		 */
		~Addon();
		
		// MARK: - Functions (const)
		
		/*!
		 Get a localised string from the add-on resources
		 @param itemIndex The index of the string item in the resource
		 @param resourceIndex The resource index
		 @return The localised string
		 */
		speckle::utility::String getLocalString(short itemIndex, short resourceIndex) const;
		/*!
		 Get the active project
		 @return The active project (nullptr = no open project)
		 */
		std::weak_ptr<Project> getActiveProject() const;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Set the add-on name
		 @param nm The add-on name
		 */
		void setName(const speckle::utility::String& nm) { App::name = nm; }
		/*!
		 Publish an event from an external source to subscribers
		 @param event The event to publish
		 */
		void publishExternal(const active::event::Event& event);
		/*!
		 Audit the participant environment to ensure it can function
		 @return True if the participant is able to function
		 */
		bool audit() override;
		/*!
		 Attach participant components to the app (as required)
		 @return True if the participant is able to function
		 */
		bool attach() override;
		/*!
		 Initialise the participant operation
		 @return True if the participant is able to continue
		 */
		bool start() override;
		/*!
		 Shut down event handling
		 */
		void stop() override;
		
	protected:
		/*!
		 Log a callback into the add-on (allows checking of re-entry)
		 @param initialise True to force this callback to be treated as the initial call (not re-entry)
		 @return True if the callback can continue (false on error)
		 */
		bool logCallback(bool initialise = true);
		/*!
		 Preprocess an external event (allowing key add-on operations to act before other subscribers)
		 @param event An incoming event
		 @return True if the event should be closed, i.e. not passed to other subscribers
		 */
		virtual bool preprocessEvent(const active::event::Event& event);
		/*!
		 Postprocess an external event (allowing key add-on operations to act after all other subscribers are complete)
		 @param event An incoming (completed) event
		 */
		virtual void postprocessEvent(const active::event::Event& event);
		/*!
		 Make a new new project. Allows Addon subclasses to define a Project subclass with additional functions/databases
		 @return A new project instance
		 */
		virtual std::shared_ptr<Project> makeProject() const;
		
	private:
			///The active project
		std::shared_ptr<Project> m_activeProject;
			///The depth of nested callbacks - the root call starts at depth 0 (important for some entry-point initialisation)
		uint32_t m_callDepth = 0;
	};
	
	
	/*!
	 Get an object representing the add-on instance
	 @return The active application instance (nullptr if the addon is not running)
	 */
	speckle::environment::Addon* addon();

}

#endif	//SPECKLE_ENVIRONMENT_ADDON
