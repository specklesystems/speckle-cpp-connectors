#ifndef SPECKLE_ENVIRONMENT_ADDON
#define SPECKLE_ENVIRONMENT_ADDON

#include "Active/Environment/App.h"
#include "Speckle/Utility/String.h"

namespace speckle::environment {
		
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
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Addon(const App& source) : App{source} {}
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
		 Determine if the active document is shared (in collaborative environments)
		 @return True if the active document is shared
		 */
		bool isSharedDocument() const;
		
		// MARK: - Functions (mutating)
		
		/*!
		 Set the add-on name
		 @param nm The add-on name
		 */
		void setName(const speckle::utility::String& nm) { name = nm; }
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
		
	private:
		uint32_t m_callDepth = 0;
	};
	
	
	/*!
	 Get an object representing the add-on instance
	 @return The active application instance (nullptr if the addon is not running)
	 */
	speckle::environment::Addon* addon();

}

#endif	//SPECKLE_ENVIRONMENT_ADDON
