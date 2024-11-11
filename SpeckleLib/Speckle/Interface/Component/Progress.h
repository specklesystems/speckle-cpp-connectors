#ifndef SPECKLE_INTERFACE_PROGRESS
#define SPECKLE_INTERFACE_PROGRESS

#include "Speckle/Utility/String.h"

#include <memory>

namespace speckle::interfac {
	
	/*!
	 Interface component to provide progress feedback during some long process to the user, e.g. a progress meter
	 
	 The structure of this component is intended for a host-wide singleton, i.e. a modal process that locks out other processes, rather than
	 simply (say) a progress bar as a component in a dialog/palette. This reflects the behaviour of Archicad but may be adapted to other
	 hosts
	 */
	class Progress {
	public:

		// MARK: - Types
		
			///Shared pointer
		using Shared = std::shared_ptr<Progress>;

		// MARK: - Constructors
		
		Progress(const Progress&) = delete;
		/*!
		 Destructor
		 */
		~Progress();

		/*!
		 Get a progress component instance
		 @param title The process title, i.e. for the overall operation. Ignored if an instance is already in operation
		 @param stages The number of stages for completing the overall operation. Ignored if an instance is already in operation
		 @return The progress instance - can be an active progress component if it has already been started (nullptr on failure)
		 */
		static Shared getInstance(const utility::String& title = {}, size_t stages = 1);
		
		// MARK: - Operators
		
		/*!
		 Addition with assignment
		 @param toAdd The number of steps to add to the progress of the current stage
		 */
		void operator+= (size_t toAdd);
		/*!
		 Increment operator (increment the the progress of the current stage)
		 */
		void operator++ ();
		
		// MARK: - Functions (const)

		/*!
		 Determine if the process has been cancelled, e.g. the user cancelling
		 @return True if the process has been cancelled
		 */
		bool isCancelled();
		
		// MARK: - Functions (mutating)

		/*!
		 Start the next stage
		 @param title The stage title
		 @param stepCount The number of steps in this stage
		 @param showPercentage True to estimate/display the process completion percentage
		 */
		void startStage(const utility::String& title, size_t stepCount, bool showPercentage = true);
		
	private:
		/*!
		 Default constructor
		 */
		Progress() {}
	};
	
}

#endif	//SPECKLE_INTERFACE_PROGRESS
