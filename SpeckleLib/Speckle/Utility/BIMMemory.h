#ifndef SPECKLE_UTILITY_BIM_MEMORY
#define SPECKLE_UTILITY_BIM_MEMORY

#include <memory>

namespace speckle::utility {
	
	/*!
	 BIM memory functions
	*/
	class BIMMemory {
	public:
		/*!
		 Get the size of an allocated pointer block
		 @param pointer The target pointer
		 @return The allocated pointer size
		 */
		static size_t getPtrSize(void* pointer);
	};
	
}

#endif	//SPECKLE_UTILITY_BIM_MEMORY
