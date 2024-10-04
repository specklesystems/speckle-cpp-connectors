#include "Speckle/Serialise/Units/LengthUnit.h"

#include <algorithm>
#include <map>

using namespace active::measure;
using namespace active::utility;

using enum LengthType;

namespace {
	
		//All recognised encoding types
	std::map<LengthType, String> lengthNames = {
		{ millimetre, "mm" },
		{ centimetre, "cm" },
		{ metre, "m" },
		{ kilometre, "km" },
		{ inch, "in" },
		{ foot, "ft" },
		{ yard, "yd" },
		{ mile, "ml" },
	};

}  // namespace

/*--------------------------------------------------------------------
	Convert a length unit name to a type
 
	name: The length unit name
 
	return: The length unit type (nullopt if no match found)
 --------------------------------------------------------------------*/
std::optional<LengthType> speckle::serialise::lengthUnitFromName(const String& name) {
	if (auto match = std::find_if(lengthNames.begin(), lengthNames.end(), [&](const auto& i){ return (i.second == name); });
			match != lengthNames.end()) {
		return match->first;
	}
	return std::nullopt;
} //encodingFromName


/*--------------------------------------------------------------------
	Convert a length unit type to a name
 
	lengthType: The length unit type
 
	return: The name of the length unit type (nullopt if no match found)
  --------------------------------------------------------------------*/
std::optional<String> speckle::serialise::nameFromLengthUnit(LengthType lengthType) {
	if (auto match = lengthNames.find(lengthType); match != lengthNames.end()) {
		return match->second;
	}
	return std::nullopt;
} //nameFromEncoding
