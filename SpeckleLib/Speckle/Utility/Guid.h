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

	///Hashing for Guid class, e.g. to use as a key in unordered_map
template<>
struct std::hash<speckle::utility::Guid> {
	std::size_t operator() (const active::utility::Guid& guid) const {
		return static_cast<std::size_t>(guid.raw().first | std::rotr(guid.raw().second, 8 * sizeof(uint64_t)));
	}
};

	///Hashing for a Guid pair, e.g. to use as a key in unordered_map
template<>
struct std::hash<std::pair<speckle::utility::Guid, speckle::utility::Guid>> {
	std::size_t operator() (const std::pair<speckle::utility::Guid, speckle::utility::Guid>& pair) const {
		return std::hash<speckle::utility::Guid>()(pair.first) | std::rotr(std::hash<speckle::utility::Guid>()(pair.second), 8 * sizeof(std::size_t));
	}
};

#endif	//SPECKLE_UTILITY_GUID
