#ifndef SPECKLE_UTILITY_GUID_64
#define SPECKLE_UTILITY_GUID_64

#include "Speckle/Utility/String.h"

namespace speckle::utility {
		
	/*!
		A class representing a 64-bit GUID with base64 text encoding
	*/
	class Guid64 {
	public:
		
		// MARK: - Types
		
		using Raw = uint64_t;
			///Optional
		using Option = std::optional<Guid64>;
		
		// MARK: - Constructors
		
		/*!
			Default constructor
			@param autoGenerate True to generate a guid value on construction
		*/
		Guid64(bool autoGenerate = false);
		/*!
			Constructor
			@param base64String The guid in string form (encoded as base64)
		*/
		explicit Guid64(const String& base64String);
		
		// MARK: - Operators

		friend auto operator<=>(const Guid64&, const Guid64&) = default;
		friend bool operator==(const Guid64&, const Guid64&) = default;
		friend bool operator!=(const Guid64&, const Guid64&) = default;
		friend bool operator<(const Guid64&, const Guid64&) = default;
		/*!
			Conversion operator
			@return The guid as a string
		*/
		operator String() const { return string(); }
		/*!
			Conversion operator
			@return The guid as a std::string
		*/
		operator std::string() const { return string(); }
		/*!
			Conversion operator
			@return True if the guid has a value (non-nil)
		*/
		operator bool() const { return (m_value != 0); }
		
		// MARK: - Functions (const)
		
		/*!
			Return the raw (native) representation of the guid
			@return The raw native representation
		*/
		const Raw& raw() const { return m_value; }
		/*!
			Return a string representation of the guid
			@return A string representation
		*/
		String string() const;
		
		// MARK: - Functions (mutating)
		
		/*!
			Reset the guid with a new value
		*/
		void reset();
		/*!
			Clear the guid value
		*/
		void clear() { m_value = {}; }
		
	private:
			//The guid value
		Raw m_value = {};
	};
	
}

	///Hashing for Guid64 class, e.g. to use as a key in unordered_map
template<>
struct std::hash<speckle::utility::Guid64> {
	std::size_t operator() (const speckle::utility::Guid64& guid) const {
		return static_cast<std::size_t>(guid.raw());
	}
};

#endif	//SPECKLE_UTILITY_GUID_64
