#include "Speckle/Environment/Addon.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace speckle::environment;
using namespace speckle::utility;

namespace {
	
		//An object representing the active addon
	Addon* m_addonInstance = nullptr;

}

/*--------------------------------------------------------------------
	Constructor (NB: this object is assumed to be the active instance)
 
	identity: Optional name/ID for the subscriber
  --------------------------------------------------------------------*/
Addon::Addon(const active::utility::NameID& identity) : App{identity, true} {
	m_addonInstance = this;
} //Addon::Addon


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Addon::~Addon() {
	m_addonInstance = nullptr;
} //Addon::~Addon


/*--------------------------------------------------------------------
	Get a localised string from the add-on resources
 
	itemIndex: The index of the string item in the resource
	resourceIndex: The resource index
 
	return: The localised string
  --------------------------------------------------------------------*/
String Addon::getLocalString(short itemIndex, short resourceIndex) const {
#ifdef ARCHICAD
	GS::UniString string;
	RSGetIndString(&string, itemIndex, resourceIndex, ACAPI_GetOwnResModule());
	return string;
#else
	return String{}
#endif
} //Addon::getLocalString


/*--------------------------------------------------------------------
	Publish an event from an external source to subscribers
 
	event: The event to publish
  --------------------------------------------------------------------*/
void Addon::publishExternal(const active::event::Event& event) {
	if (!logCallback())
		return;
	try {
		publish(event);
	} catch (...) {
		//Add error logging in future
	}

} //Addon::publishExternal


/*--------------------------------------------------------------------
	Audit the participant environment to ensure it can function
 
	return: True if the participant is able to function
  --------------------------------------------------------------------*/
bool Addon::audit() {
#ifdef ARCHICAD
	ACAPI_KeepInMemory(true);
#endif
	return App::audit();
} //Addon::audit


/*--------------------------------------------------------------------
	Attach participant components to the app (as required)
 
	return: True if the participant is able to function
  --------------------------------------------------------------------*/
bool Addon::attach() {
#ifdef ARCHICAD
	ACAPI_KeepInMemory(true);
#endif
	return App::attach();
} //Addon::attach


/*--------------------------------------------------------------------
	Initialise the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool Addon::start() {
		//Add add-on functionality as required
	return App::start();
} //Addon::start


/*--------------------------------------------------------------------
	Shut down event handling
  --------------------------------------------------------------------*/
void Addon::stop() {
		//Add add-on functionality as required
	App::stop();
} //Addon::stop


/*--------------------------------------------------------------------
	Get an object representing the parent process/application
 
	return: The active application instance
 --------------------------------------------------------------------*/
speckle::environment::Addon* speckle::environment::addon() {
	return m_addonInstance;
} //speckle::environment::addon


/*--------------------------------------------------------------------
	Log a callback into the add-on (allows checking of re-entry)
 
	initialise: True to force this callback to be treated as the initial call (not re-entry)
 
 	return: True if the callback can continue (false on error)
  --------------------------------------------------------------------*/
bool speckle::environment::Addon::logCallback(bool initialise) {
	if (initialise)
		m_callDepth = 1;
	else
		++m_callDepth;
	try {
#ifdef ARCHICAD
		ACAPI_KeepInMemory(true);
#endif
		//Add additional house-keeping functionality as required, e.g. retrieval of preferences
	} catch (...) {
		return false;
	}
	return true;
} //Addon::publishExternalEvent
