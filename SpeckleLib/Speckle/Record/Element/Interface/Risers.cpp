#include "Speckle/Record/Element/Interface/Risers.h"

#include "Speckle/Record/Element/StairRiser.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/Record/Element/Interface/Assembly/Path.h"
#include "Speckle/Utility/BIMMemory.h"

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

namespace speckle::record::element {

	class Risers::Data {
	public:
		friend class Risers;

		std::vector<StairRiser> risers;
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Risers::Risers() {
} //Risers::Risers


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Risers::Risers(const Risers& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //Risers::Risers


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Risers::~Risers() {}


/*--------------------------------------------------------------------
	Get the number of risers
 
	return. The number of risers (0 on error)
  --------------------------------------------------------------------*/
size_t Risers::getRiserCount() const {
	confirmPart(getPartFilter());
	return m_data ? m_data->risers.size() : 0;
} //Risers::getRiserCount


/*--------------------------------------------------------------------
	Get a stair riser
 
	index: The index of the required riser
 
	return: The requested riser, nullptr on error
  --------------------------------------------------------------------*/
StairRiser* Risers::getRiser(size_t index) const {
	confirmPart(getPartFilter());
	return (m_data && (index < m_data->risers.size())) ? &m_data->risers[index] : nullptr;
} //Risers::getRiser


/*--------------------------------------------------------------------
	Return the bits for the part filter required to load the data necessary to build this object
 
	return: The required filter bits
  --------------------------------------------------------------------*/
Part::filter_bits Risers::getPartFilter() const {
#ifdef ARCHICAD
	return APIMemoMask_StairRiser;
#endif
} //Risers::getPartFilter


/*--------------------------------------------------------------------
	Determine if the element memo content has been validated (elements must override according to requirements)
 
	return: True if the element memo content is valid
  --------------------------------------------------------------------*/
bool Risers::isPartValid() const {
	return m_data && !m_data->risers.empty();
} //Risers::isPartValid


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Risers::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
	
} //Risers::loadMemo


/*--------------------------------------------------------------------
	Send the element part back to a memo structure for storage (elements must override according to requirements)
 
	memo: The memo to carry the data
 
	return: True if the data was successfully sent
  --------------------------------------------------------------------*/
bool Risers::send(Memo* memo) const {
		//TODO: Complete when required
	return false;
} //Risers::send


/*--------------------------------------------------------------------
	Receive the element memo data from a memo structure (elements must override according to requirements)
 
	memo: The memo carrying the data
 
	return: True if the data was successfully received
  --------------------------------------------------------------------*/
bool Risers::receive(const Memo& memo) const {
#ifdef ARCHICAD
	if (!memo || (memo.root()->stairRisers == nullptr))
		return false;
	if (m_data)
		m_data->risers.clear();
	else
		m_data = std::make_unique<Data>();
		//Confirm that required data is present in the memo
	auto riserPtr = memo.root()->stairRisers;
	if (riserPtr == nullptr)
		return false;
		//Determine available item count
	auto riserCount = BIMMemory::getPtrSize(riserPtr) / sizeof(API_StairRiserType);
	if (riserCount == 0)
		return false;
	for (size_t n = 0 ; n < riserCount; ++n)
		m_data->risers.emplace_back(StairRiser{riserPtr[n], getTableID()});
	setMemoLoaded(true);
#endif
	return true;
} //Risers::receive
