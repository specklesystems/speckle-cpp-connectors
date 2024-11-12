#include "Speckle/Interface/Component/Progress.h"

#include <mutex>

#ifdef ARCHICAD
#ifdef ServerMainVers_2600
#include "ACAPI_Interface.h"
#endif
#endif

using namespace speckle::interfac;
using namespace speckle::utility;

namespace {
	
		///The progress class instance - a weak reference is held so the object is released when active users complete
	std::weak_ptr<Progress> m_instance;
		///Mutex controlling management of the progress instance
	std::mutex m_mutex;
	
}

/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Progress::~Progress() {
#ifdef ARCHICAD
#ifdef ServerMainVers_2600
	ACAPI_ProcessWindow_CloseProcessWindow();
#else
	ACAPI_Interface(APIIo_CloseProcessWindowID);
#endif
#endif
} //Progress::~Progress


/*--------------------------------------------------------------------
	Get a progress component instance
 
	title: The process title, i.e. for the overall operation. Ignored if an instance is already in operation
	stages: The number of stages for completing the overall operation. Ignored if an instance is already in operation
 
	return: The progress instance - can be an active progress component if it has already been started (nullptr on failure)
  --------------------------------------------------------------------*/
Progress::Shared Progress::getInstance(const String& title, size_t stages) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	if (auto instance = m_instance.lock(); instance)
		return instance;
#ifdef ARCHICAD
	GS::UniString gsTitle{title};
	auto phases = static_cast<Int32>(stages);
#ifdef ServerMainVers_2600
	if (ACAPI_ProcessWindow_InitProcessWindow(&gsTitle, &phases) != NoError)
#else
	if (ACAPI_Interface(APIIo_InitProcessWindowID, &gsTitle, &phases) != NoError)
#endif
		return nullptr;
#endif
	auto result = Progress::Shared{new Progress};
	m_instance = result;
	return result;
} //Progress::getInstance


/*--------------------------------------------------------------------
	Addition with assignment
 
	toAdd: The number of steps to add to the progress of the current stage
  --------------------------------------------------------------------*/
void Progress::operator+= (size_t toAdd) {
#ifdef ARCHICAD
	auto incVal = static_cast<Int32>(toAdd);
#ifdef ServerMainVers_2600
	ACAPI_ProcessWindow_IncProcessValue(&incVal);
#else
	ACAPI_Interface(APIIo_IncProcessValueID, &incVal);
#endif
#endif
} //Progress::operator+=


/*--------------------------------------------------------------------
	Increment operator (increment the the progress of the current stage)
  --------------------------------------------------------------------*/
void Progress::operator++ () {
	operator+=(1);
} //Progress::operator++


/*--------------------------------------------------------------------
	Start the next stage
 
	title: The stage title
	stepCount: The number of steps in this stage
	showPercentage: True to estimate/display the process completion percentage
  --------------------------------------------------------------------*/
void Progress::startStage(const String& title, size_t stepCount, bool showPercentage) {
#ifdef ARCHICAD
	GS::UniString gsTitle{title};
	auto maxVal = static_cast<Int32>(stepCount);
#ifdef ServerMainVers_2600
	ACAPI_ProcessWindow_SetNextProcessPhase(&gsTitle, &maxVal, &showPercentage);
#else
	ACAPI_Interface(APIIo_SetNextProcessPhaseID, &gsTitle, &maxVal, &showPercentage);
#endif
#endif
} //Progress::startStage


/*--------------------------------------------------------------------
	Determine if the process has been cancelled, e.g. the user cancelling
 
	return: True if the process has been cancelled
  --------------------------------------------------------------------*/
bool Progress::isCancelled() {
#ifdef ARCHICAD
#ifdef ServerMainVers_2600
	return (ACAPI_ProcessWindow_IsProcessCanceled() != NoError);
#else
	return (ACAPI_Interface(APIIo_IsProcessCanceledID) != NoError);
#endif
#endif
} //Progress::isCancelled
