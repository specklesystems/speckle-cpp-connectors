#include "Speckle/Record/Element/Interface/Treads.h"

#include "Speckle/Record/Element/StairTread.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/Record/Element/Interface/Assembly/Path.h"
#include "Speckle/Utility/BIMMemory.h"

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

namespace speckle::record::element {

	class Treads::Data {
	public:
		friend class Treads;

		std::vector<StairTread> treads;
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Treads::Treads() {
} //Treads::Treads


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Treads::Treads(const Treads& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Treads::Treads


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Treads::~Treads() {}


/*--------------------------------------------------------------------
	Get the number of treads
 
	return. The number of treads (0 on error)
  --------------------------------------------------------------------*/
size_t Treads::getTreadCount() const {
	confirmPart(getPartFilter());
	return m_data ? m_data->treads.size() : 0;
} //Treads::getTreadCount


/*--------------------------------------------------------------------
	Get a stair tread
 
	index: The index of the required tread
 
	return: The requested tread, nullptr on error
  --------------------------------------------------------------------*/
StairTread* Treads::getTread(size_t index) const {
	confirmPart(getPartFilter());
	return (m_data && (index < m_data->treads.size())) ? &m_data->treads[index] : nullptr;
} //Treads::getTread


/*--------------------------------------------------------------------
	Return the bits for the part filter required to load the data necessary to build this object
 
	return: The required filter bits
  --------------------------------------------------------------------*/
Part::filter_bits Treads::getPartFilter() const {
#ifdef ARCHICAD
	return APIMemoMask_StairTread;
#endif
} //Treads::getPartFilter


/*--------------------------------------------------------------------
	Determine if the element memo content has been validated (elements must override according to requirements)
 
	return: True if the element memo content is valid
  --------------------------------------------------------------------*/
bool Treads::isPartValid() const {
	return m_data && !m_data->treads.empty();
} //Treads::isPartValid


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Treads::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
	
} //Treads::loadMemo


/*--------------------------------------------------------------------
	Send the element part back to a memo structure for storage (elements must override according to requirements)
 
	memo: The memo to carry the data
 
	return: True if the data was successfully sent
  --------------------------------------------------------------------*/
bool Treads::send(Memo* memo) const {
		//TODO: Complete when required
	return false;
} //Treads::send


/*--------------------------------------------------------------------
	Receive the element memo data from a memo structure (elements must override according to requirements)
 
	memo: The memo carrying the data
 
	return: True if the data was successfully received
  --------------------------------------------------------------------*/
bool Treads::receive(const Memo& memo) const {
#ifdef ARCHICAD
	if (!memo || (memo.root()->stairTreads == nullptr))
		return false;
	if (m_data)
		m_data->treads.clear();
	else
		m_data = std::make_unique<Data>();
		//Confirm that required data is present in the memo
	auto treadPtr = memo.root()->stairTreads;
	if (treadPtr == nullptr)
		return false;
		//Determine available item count
	auto treadCount = BIMMemory::getPtrSize(treadPtr) / sizeof(API_StairTreadType);
	if (treadCount == 0)
		return false;
	for (size_t n = 0 ; n < treadCount; ++n)
		m_data->treads.emplace_back(StairTread{treadPtr[n], getTableID()});
	setMemoLoaded(true);
#endif
	return true;
} //Treads::receive
