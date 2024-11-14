#ifndef SPECKLE_ENVIRONMENT_HOST
#define SPECKLE_ENVIRONMENT_HOST

#include "Speckle/Utility/String.h"

namespace speckle::environment {
	
	/*!
		Class to represent a host application (for plugin contexts)
	*/
	class Host {
	public:

		// MARK: Constructors

		/*!
		 Default constructor
		 */
		Host();

		// MARK: Functions (const)
		
		/*!
		 Switch to the model view
		 @param isSelectionOnly True to display just the selected elements
 		 @return True if the model view is active
		*/
		bool makeModelViewActive(bool isSelectionOnly = false) const;
		/*!
		 Zoom the active view to fit the content bounds
		 @param isSelectionOnly True to zoom to fit just the current selection
		*/
		void zoomToFit(bool isSelectionOnly = false) const;
		/*!
		 Display an alert dialog
		 @param message The alert message
		*/
		void displayAlert(const speckle::utility::String& message) const;
		
		// MARK: Functions (mutating)
		
	};

	/*!
	 Get an object representing the host instance
	 @return The active host instance (nullptr if the host is not running)
	 */
	speckle::environment::Host* host();

}

#endif //SPECKLE_ENVIRONMENT_HOST
