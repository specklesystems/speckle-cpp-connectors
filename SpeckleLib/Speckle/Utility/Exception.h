#ifndef SPECKLE_UTILITY_EXCEPTION
#define SPECKLE_UTILITY_EXCEPTION

#include "Speckle/Utility/String.h"

#include <stdexcept>

namespace speckle::utility {
	
	/*!
	 Record index class
	*/
	class Exception : public std::runtime_error {
	public:

		// MARK: - Types
		
		using base = std::runtime_error;

		// MARK: - Constructors

		using base::base;
		
		/*!
		 Constructor
		 @param message The exception message
		 */
		Exception(const String& message) : base{message.operator std::string()} {}

		// MARK: - Operators
		
		using base::operator=;
	};
	
}

#endif	//SPECKLE_UTILITY_EXCEPTION
