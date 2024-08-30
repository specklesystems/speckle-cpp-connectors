#include "Speckle/Utility/Guid64.h"

#include "Active/Serialise/Generic/Base64Transport.h"
#include "Active/Utility/BufferIn.h"
#include "Active/Utility/BufferOut.h"

#include <random>

using namespace active::serialise;
using namespace speckle::utility;

namespace {
	
	constexpr unsigned long long maxInt64 = std::numeric_limits<unsigned long long>::max();
	static const String nullBase64{"AAAAAAAAAAA"};
	
	uint64_t randomInt64() {
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<unsigned long long> dis(0, maxInt64);
		return static_cast<uint64_t>(dis(gen));
	}
	
}  // namespace

/*--------------------------------------------------------------------
	Default constructor
 
	autoGenerate: True to generate a guid value on construction
  --------------------------------------------------------------------*/
Guid64::Guid64(bool autoGenerate) {
	if (autoGenerate)
		reset();
} //Guid64::Guid64


/*--------------------------------------------------------------------
	Constructor
 
	base64String: The guid in string form (encoded as base64)
  --------------------------------------------------------------------*/
Guid64::Guid64(const String& base64String) {
	using namespace active::utility;
	auto incoming{base64String};
		//We need 10 characters for a valid guid
	if (incoming.size() != 11)
		return;
		//Convert the incoming base64 to a 64-bit integer
	BufferIn source{incoming};
	Base64Transport transport;
	if (!transport.receive(Memory{m_value}, source))
		return;
	m_value = Memory::fromBigEndian(m_value);
} //Guid64::Guid64


/*--------------------------------------------------------------------
	Return a string representation of the guid
 
	return: A string representation
  --------------------------------------------------------------------*/
String Guid64::string() const {
	String base64String;
	using namespace active::utility;
		//Convert the value to base64
	auto val = Memory::toBigEndian(m_value);
	if (!Base64Transport().send(Memory{val}, base64String) || (base64String.length() != 12))
		base64String = nullBase64;
	else
		base64String.erase(11);	//Remove the trailing '='
	return base64String;
} //Guid64::string


/*--------------------------------------------------------------------
	Reset the guid with a new value
  --------------------------------------------------------------------*/
void Guid64::reset() {
	m_value = randomInt64();
} //Guid64::reset
