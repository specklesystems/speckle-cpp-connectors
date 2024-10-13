#ifndef SPECKLE_SERIALISE_STR256
#define SPECKLE_SERIALISE_STR256

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Item/Wrapper/ValueOptionWrap.h"
#include "Active/Utility/BufferOut.h"

namespace active::serialise {
	
	constexpr size_t str256Size = 256;
	
		///NB: This is primarily for Archicad that still uses fixed 256-bytes char array strings in some contexts
	using Str256 = std::array<char, str256Size>;
	
	// MARK: - Specialisations for Str256

	/*!
		Import the object from the specified string (specialisation for Str256)
		@param source The string to read
		@return True if the data was successfully read
	*/
	template<> inline
	bool ValueWrap<Str256>::read(const utility::String& source) {
		source.writeUTF8(active::utility::BufferOut{get()});
		return true;
	} //ValueWrap<Str256>::read
	

	/*!
		Import the object from the specified string (specialisation for Str256)
		@param source The string to read
		@return True if the data was successfully read
	*/
	template<> inline
	bool ValueWrap<std::optional<Str256>>::read(const utility::String& source) {
		base::get() = Str256{};
		source.writeUTF8(active::utility::BufferOut{*base::get()});
		return true;
	} //ValueWrap<Str256>::read

	
	/*!
		Export the object to the specified string (specialisation for Str256)
		@param dest The string to write the data to
		@return True if the data was successfully written
	*/
	template<> inline
	bool ValueWrap<Str256>::write(utility::String& dest) const {
		dest.assign(reinterpret_cast<const char*>(&get()), str256Size);
		return true;
	} //ValueWrap<Str256>::write
	
}

#endif	//SPECKLE_SERIALISE_STR256
