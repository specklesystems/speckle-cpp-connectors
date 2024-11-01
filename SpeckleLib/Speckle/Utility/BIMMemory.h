#ifndef SPECKLE_UTILITY_BIM_MEMORY
#define SPECKLE_UTILITY_BIM_MEMORY

#include <memory>

namespace speckle::utility {
	
	/*!
	 BIM memory functions
	 
	 NB: These functions are primarily relevant to C-based BIM APIs that require low-level memory management, e.g. Archicad
	*/
	class BIMMemory {
	public:
		/*!
		 Get the size of an allocated pointer block
		 @param pointer The target pointer
		 @return The allocated pointer size
		 */
		static size_t getPtrSize(void* pointer);
		/*!
		 Set a mask in a specified byte, i.e. fill with 1s
		 @param byte A pointer to the target byte
		 */
		static void setMask(void* byte) { *(reinterpret_cast<unsigned char*>(byte)) = 0xFF; }
	};
	
}

#endif	//SPECKLE_UTILITY_BIM_MEMORY
