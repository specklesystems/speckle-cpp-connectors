#ifndef SPECKLE_UTILITY_GUID
#define SPECKLE_UTILITY_GUID

#include "Active/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

namespace speckle::utility {
	
	/*!
	 Record index class
	*/
	class Guid : public active::utility::Guid {
	public:

		// MARK: - Types
		
		using base = active::utility::Guid;

		// MARK: - Constructors

		using base::base;
		
		/*!
		 Constructor
		 @param source The guid to copy
		 */
		Guid(const active::utility::Guid& source) : base{source} {}

#ifdef ARCHICAD
		/*!
		 Constructor
		 @param guid An Archicad guid
		*/
		Guid(const GS::Guid& guid);
		/*!
		 Constructor
		 @param guid An Archicad guid
		*/
		Guid(const API_Guid& guid) : Guid(APIGuid2GSGuid(guid)) {}
#endif

		// MARK: - Operators
		
		using base::operator=;
		
		/*!
			Conversion operator
			@return The guid as a string
		*/
		operator active::utility::String() const { return base::string(); }
		/*!
			Conversion operator
			@return The guid as a string
		*/
		operator speckle::utility::String() const { return base::string(); }
#ifdef ARCHICAD
		/*!
		 Conversion operator
		 @return An equivalent Archicad guid
		 */
		operator GS::Guid() const;
		/*!
		 Conversion operator
		 @return An equivalent Archicad guid
		 */
		operator API_Guid() const;
#endif
	};
	
}

#endif	//SPECKLE_UTILITY_GUID
