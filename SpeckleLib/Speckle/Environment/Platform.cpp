#include "Speckle/Environment/Platform.h"

#include <iostream>

using namespace speckle::environment;
using namespace speckle::utility;

namespace {
	
		//An object representing the active addon
	Platform m_platformInstance;

}

/*--------------------------------------------------------------------
	Write a message to the console

	message: The message to write
 --------------------------------------------------------------------*/
void Platform::writeToConsole(const active::utility::String& message) {
#ifdef WINDOWS
		//NB: std::cout doesn't write to the console in Windows by default
	OutputDebugString((LPCTSTR)message.operator std::u16string().data());
#else
	std::cout << message.data();
#endif
} //Platform::writeToConsole


/*--------------------------------------------------------------------
	Open a URL
 
	URL: The URL to open
 --------------------------------------------------------------------*/
void Platform::openURL(const active::utility::String& URL) {
#if WINDOWS
	std::system((String{"start "} + URL).data());
#elif __APPLE__
	std::system((String{"open "} + URL).data());
#elif __linux__
	std::system((String{"xdg-open"} + url).data());
#endif
} //Platform::openURL


/*--------------------------------------------------------------------
	Get an object representing the parent process/application
 
	return: The active application instance
 --------------------------------------------------------------------*/
speckle::environment::Platform* speckle::environment::platform() {
	return &m_platformInstance;
} //speckle::environment::platform
