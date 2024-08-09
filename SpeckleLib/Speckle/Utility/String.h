#ifndef SPECKLE_UTILITY_STRING
#define SPECKLE_UTILITY_STRING

#include "Active/Utility/Guid.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

namespace speckle::utility {
	
	/*!
	 Record index class
	*/
	class String : public active::utility::String {
	public:

		// MARK: - Types
		
		using base = active::utility::String;
			///Optional size type (used to indicate an unspecified or non-existant position)
		using sizeOption = base::sizeOption;

		// MARK: - Constructors

		using base::base;

#ifdef ARCHICAD
		/*!
		 Constructor
		 @param string An Archicad string
		 @param howMany The number of chars to copy (nullopt for full length)
		 */
		String(const GS::UniString& string, sizeOption howMany = std::nullopt) :
				String{reinterpret_cast<const char16_t*>(string.ToUStr().operator const unsigned short *()), howMany} {}
		/*!
		 Constructor
		 @param string An Archicad string
		 @param howMany The number of chars to copy (nullopt for full length)
		 */
		String(const GS::String& string, sizeOption howMany = std::nullopt) :
				String{string.ToCStr(), base::getValidByteCount(string.ToCStr(), std::nullopt, howMany)} {}
		/*!
		 Constructor
		 @param string An Archicad string
		 @param howMany The number of chars to copy (nullopt for full length)
		 */
		String(const GS::uchar_t* string, sizeOption howMany = std::nullopt) : String{reinterpret_cast<const char16_t*>(string), howMany} {}
		/*!
		 Constructor
		 @param string An Archicad string
		 */
		String(const IO::Name& string) : String{string.ToString()} {}
#endif

		// MARK: - Operators
		
		using base::operator=;
		
#ifdef ARCHICAD
		/*!
		 Assignment operator
		 @param string An Archicad string
		 @return A reference to this
		 */
		String& operator= (const GS::UniString& string) {
			assign(reinterpret_cast<const char*>(string.ToUStr().operator const unsigned short *()), std::nullopt, std::nullopt, UTF16);
			return *this;
		}
		/*!
		 Assignment operator
		 @param string An Archicad string
		 @return A reference to this
		 */
		String& operator= (const GS::String& string) {
			assign(string.ToCStr(), std::nullopt, std::nullopt, UTF8);
			return *this;
		}
		/*!
		 Assignment operator
		 @param string An Archicad string
		 @return A reference to this
		 */
		String& operator= (const GS::uchar_t* string) {
			assign(reinterpret_cast<const char*>(string), std::nullopt, std::nullopt, UTF16);
			return *this;
		}
		/*!
		 Assignment operator
		 @param string An Archicad string
		 @return A reference to this
		 */
		String& operator= (const IO::Name& string) { return operator=(string.ToString()); }
		/*!
		 Conversion operator
		 @return An equivalent Archicad string
		 */
		operator GS::UniString() const { return GS::UniString{data(), CC_UTF8}; }
		/*!
		 Conversion operator
		 @return An equivalent Archicad string
		 */
		operator IO::Name() const { return IO::Name{operator::GS::UniString()}; }
#endif
	
	// MARK: - Functions (const)
	
#ifdef ARCHICAD
		/*!
		 Write this string to an Archicad string
		 @param string The Archicad string
		 @param howMany The number of chars to write (nullopt for full length)
		 */
		void writeTo(GS::uchar_t* string, sizeOption howMany = std::nullopt);
#endif
	};
	
}

#endif	//SPECKLE_UTILITY_STRING
