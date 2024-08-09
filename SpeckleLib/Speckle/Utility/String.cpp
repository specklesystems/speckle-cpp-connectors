#include "Speckle/Utility/String.h"

#include "Active/Utility/BufferOut.h"
#include "Active/Utility/Memory.h"

using namespace speckle::utility;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Write this string to an Archicad string
 
	string: The Archicad string
	howMany: The number of chars to write (nullopt for full length)
  --------------------------------------------------------------------*/
void String::writeTo(GS::uchar_t* string, sizeOption howMany) {
		//NB: We can only assume the caller has allocated sufficient space for the specified string length
	base::writeTo(active::utility::Memory{reinterpret_cast<void*>(string), (howMany.value_or(size()) + 1) * sizeof(char16_t)}, UTF16, true, howMany);
} //String::writeTo
#endif
