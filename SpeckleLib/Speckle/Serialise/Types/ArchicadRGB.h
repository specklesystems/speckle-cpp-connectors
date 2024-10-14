#ifndef SPECKLE_SERIALISE_ARCHICAD_RGB
#define SPECKLE_SERIALISE_ARCHICAD_RGB

#ifdef ARCHICAD

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Item/Wrapper/ValueOptionWrap.h"
#include "Active/Utility/BufferOut.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <ModelMaterial.hpp>
#endif

namespace active::serialise {
	
	// MARK: - Specialisations for API_RGBColor

	/*!
	 Convert a string (encoding a 32-bit unsigned integer) to an Archicad RGB colour structure
	 @param source The source string to convert
	 @return The equivalent colour (nullopt on failure)
	 */
	std::optional<API_RGBColor> convertToAPI_RGBColor(const utility::String& source) {
		auto intValue = source.toUInt32();
		if (!intValue)
			return std::nullopt;
		API_RGBColor result{};
		*intValue >>= 8;
		result.f_red = static_cast<double>(*intValue & 0xFF) / 255.0;
		*intValue >>= 8;
		result.f_green = static_cast<double>(*intValue & 0xFF) / 255.0;
		*intValue >>= 8;
		result.f_blue = static_cast<double>(*intValue & 0xFF) / 255.0;
		return result;
	}
	
	/*!
	 Import the object from the specified string (specialisation for API_RGBColor)
	 @param source The string to read
	 @return True if the data was successfully read
	 */
	template<> inline
	bool ValueWrap<API_RGBColor>::read(const utility::String& source) {
		if (auto colour = convertToAPI_RGBColor(source); colour) {
			get() = *colour;
			return true;
		}
		return false;
	} //ValueWrap<API_RGBColor>::read
	

	/*!
	 Import the object from the specified string (specialisation for API_RGBColor)
	 @param source The string to read
	 @return True if the data was successfully read
	 */
	template<> inline
	bool ValueWrap<std::optional<API_RGBColor>>::read(const utility::String& source) {
		if (auto colour = convertToAPI_RGBColor(source); colour)
			get() = colour;
		return true;
	} //ValueWrap<API_RGBColor>::read

	
	/*!
	 Export the object to the specified string (specialisation for API_RGBColor)
	 @param dest The string to write the data to
	 @return True if the data was successfully written
	 */
	template<> inline
	bool ValueWrap<API_RGBColor>::write(utility::String& dest) const {
		dest = speckle::utility::String{(static_cast<uint32_t>(get().f_red * 255.0) << 16) |
					 ((static_cast<uint32_t>(get().f_green * 255.0)) << 8) |
					 ((static_cast<uint32_t>(get().f_blue * 255.0)))};
		return true;
	} //ValueWrap<API_RGBColor>::write
	
}

#endif	//ARCHICAD
#endif	//SPECKLE_SERIALISE_ARCHICAD_RGB
