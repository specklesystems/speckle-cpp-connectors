#include "Speckle/Record/Element/Interface/StructuredStair.h"

#include "Speckle/Record/Element/StairStructure.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/Record/Element/Interface/Assembly/Path.h"
#include "Speckle/Utility/BIMMemory.h"

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

namespace speckle::record::element {

	class StructuredStair::Data {
	public:
		friend class StructuredStair;

		std::vector<StairStructure> structures;
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
StructuredStair::StructuredStair() {
} //StructuredStair::StructuredStair


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
StructuredStair::StructuredStair(const StructuredStair& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //StructuredStair::StructuredStair


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
StructuredStair::~StructuredStair() {}


/*--------------------------------------------------------------------
	Get the number of structures
 
	return. The number of structures (0 on error)
  --------------------------------------------------------------------*/
size_t StructuredStair::getStructureCount() const {
	confirmPart(getPartFilter());
	return m_data ? m_data->structures.size() : 0;
} //StructuredStair::getStructureCount


/*--------------------------------------------------------------------
	Get a stair structure
 
	index: The index of the required structure
 
	return: The requested structure, nullptr on error
  --------------------------------------------------------------------*/
StairStructure* StructuredStair::getStructure(size_t index) const {
	confirmPart(getPartFilter());
	return (m_data && (index < m_data->structures.size())) ? &m_data->structures[index] : nullptr;
} //StructuredStair::getStructure


/*--------------------------------------------------------------------
	Return the bits for the part filter required to load the data necessary to build this object
 
	return: The required filter bits
  --------------------------------------------------------------------*/
Part::filter_bits StructuredStair::getPartFilter() const {
#ifdef ARCHICAD
	return APIMemoMask_StairStructure;
#endif
} //StructuredStair::getPartFilter


/*--------------------------------------------------------------------
	Determine if the element memo content has been validated (elements must override according to requirements)
 
	return: True if the element memo content is valid
  --------------------------------------------------------------------*/
bool StructuredStair::isPartValid() const {
	return m_data && !m_data->structures.empty();
} //StructuredStair::isPartValid


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void StructuredStair::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
	
} //StructuredStair::loadMemo


/*--------------------------------------------------------------------
	Send the element part back to a memo structure for storage (elements must override according to requirements)
 
	memo: The memo to carry the data
 
	return: True if the data was successfully sent
  --------------------------------------------------------------------*/
bool StructuredStair::send(Memo* memo) const {
		//TODO: Complete when required
	return false;
} //StructuredStair::send


/*--------------------------------------------------------------------
	Receive the element memo data from a memo structure (elements must override according to requirements)
 
	memo: The memo carrying the data
 
	return: True if the data was successfully received
  --------------------------------------------------------------------*/
bool StructuredStair::receive(const Memo& memo) const {
#ifdef ARCHICAD
	if (!memo || (memo.root()->stairStructures == nullptr))
		return false;
	if (m_data)
		m_data->structures.clear();
	else
		m_data = std::make_unique<Data>();
		//Confirm that required data is present in the memo
	auto structurePtr = memo.root()->stairStructures;
	if (structurePtr == nullptr)
		return false;
		//Determine available item count
	auto structureCount = BIMMemory::getPtrSize(structurePtr) / sizeof(API_StairStructureType);
	if (structureCount == 0)
		return false;
	for (size_t n = 0 ; n < structureCount; ++n)
		m_data->structures.emplace_back(StairStructure{structurePtr[n], getTableID()});
	setMemoLoaded(true);
#endif
	return true;
} //StructuredStair::receive
