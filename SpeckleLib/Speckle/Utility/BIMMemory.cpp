#include "Speckle/Utility/BIMMemory.h"

#ifdef ARCHICAD
#include "BM.hpp"
#endif

using namespace speckle::utility;

namespace {
	
#ifdef ARCHICAD
	/*!
	 Get the error code from the last memory operation
	 @return The last error
	 */
	GSErr getLastError() {
		return BMError();
	}
#endif

}

/*--------------------------------------------------------------------
	Get the size of an allocated pointer block
 
	pointer: The target pointer
 
	return: The allocated pointer size
  --------------------------------------------------------------------*/
size_t BIMMemory::getPtrSize(void* pointer) {
#ifdef ARCHICAD
	if (pointer == nullptr)
		return 0;
	auto size = BMGetPtrSize(reinterpret_cast<GSConstPtr>(pointer));
	if (getLastError() != NoError)
		throw; //TODO: Throw a specific exception type
	return size;
#endif
} //BIMMemory::getPtrSize
