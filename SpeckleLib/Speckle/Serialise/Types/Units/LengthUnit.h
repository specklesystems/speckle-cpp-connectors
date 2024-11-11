#ifndef SPECKLE_SERIALISE_LENGTH_UNIT
#define SPECKLE_SERIALISE_LENGTH_UNIT

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Item/Wrapper/ValueOptionWrap.h"
#include "Active/Setting/Values/Measurement/Units/LengthUnit.h"

namespace speckle::serialise {
	
	/*!
	 Convert a length unit name to a type
	 @param name The length unit name
	 @return The length unit type (nullopt if no match found)
	 */
	std::optional<active::measure::LengthType> lengthUnitFromName(const active::utility::String& name);

	
	/*!
	 Convert a length unit type to a name
	 @param lengthType The length unit type
	 @return The name of the length unit type (nullopt if no match found)
	 */
	std::optional<active::utility::String> nameFromLengthUnit(active::measure::LengthType lengthType);
	
}


namespace active::serialise {
	
	// MARK: - Specialisations for length types

	/*!
	 Read a length type value from the specified string
	 @param source The string to read
	 @return True if the data was successfully read
	*/
	inline std::pair<active::measure::LengthType, bool> readLengthTypeRefValue(const utility::String& source) {
		using enum active::measure::LengthType;
		if (auto converted = speckle::serialise::lengthUnitFromName(source); converted)
			return {*converted, true};
		return {metre, false};
	} //ValueWrap<active::measure::LengthType>::read


	/*!
	 Import the object from the specified string (specialisation for length type)
	 @param source The string to read
	 @return True if the data was successfully read
	*/
	template<> inline
	bool ValueWrap<active::measure::LengthType>::read(const utility::String& source) {
		auto result = readLengthTypeRefValue(source);
		if (result.second)
			base::get() = result.first;
		return result.second;
	} //ValueWrap<active::measure::LengthType>::read
	

	/*!
	 Import the object from the specified string (specialisation for length type)
	 @param source The string to read
	 @return True if the data was successfully read
	*/
	template<> inline
	bool ValueOptionWrap<active::measure::LengthType>::read(const utility::String& source) {
		auto result = readLengthTypeRefValue(source);
		if (result.second)
			base::get() = result.first;
		return result.second;
	} //ValueWrap<active::measure::LengthType>::read

	
	/*!
	 Export the object to the specified string (specialisation for length type)
	 @param dest The string to write the data to
	 @return True if the data was successfully written
	*/
	template<> inline
	bool ValueWrap<active::measure::LengthType>::write(utility::String& dest) const {
		if (auto name = speckle::serialise::nameFromLengthUnit(base::get()); name) {
			dest = *name;
			return true;
		}
		return false;
	} //ValueWrap<active::measure::LengthType>::write

	
	/*!
	 Export the object to the specified string (specialisation for length type)
	 @param dest The string to write the data to
	 @return True if the data was successfully written
	*/
	template<> inline
	bool ValueOptionWrap<active::measure::LengthType>::write(utility::String& dest) const {
		if (!base::get())
			return false;
		if (auto name = speckle::serialise::nameFromLengthUnit(*base::get()); name) {
			dest = *name;
			return true;
		}
		return false;
	} //ValueOptionWrap<active::measure::LengthType>::write
	
	
}

#endif	//SPECKLE_SERIALISE_LENGTH_UNIT
