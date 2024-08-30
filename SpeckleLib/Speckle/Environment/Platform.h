#ifndef SPECKLE_ENVIRONMENT_PLATFORM
#define SPECKLE_ENVIRONMENT_PLATFORM

#include "Speckle/Utility/String.h"

namespace speckle::environment {
		
	/*!
		A class representing the host platform/opterating system
	*/
	class Platform {
	public:
		
		// MARK: - Functions (const)
		
		/*!
		 Write a message to the console
		 @param message The message to write
		 */
		void writeToConsole(const active::utility::String& message);
	};
	
	
	/*!
	 Get an object representing the host platform
	 @return The host platform
	 */
	speckle::environment::Platform* platform();

}

#endif	//SPECKLE_ENVIRONMENT_PLATFORM
