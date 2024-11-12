#ifndef SPECKLE_RANDOM_SHUFFLE
#define SPECKLE_RANDOM_SHUFFLE

#include <algorithm>

namespace std {
	
		//Older Archicad API versions expect random_shuffle to be available - avoid use if possible
	template< class RandomIt, class RandomFunc >
	void random_shuffle(RandomIt first, RandomIt last, RandomFunc& r) {
		shuffle(first, last, r);
	}
	
}

#endif	//SPECKLE_RANDOM_SHUFFLE
