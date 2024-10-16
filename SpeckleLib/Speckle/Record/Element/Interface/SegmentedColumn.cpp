#include "Speckle/Record/Element/Interface/SegmentedColumn.h"

#include "Speckle/Record/Element/ColumnSegment.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/Record/Element/Interface/Assembly/Path.h"
#include "Speckle/Utility/BIMMemory.h"

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

namespace speckle::record::element {

	class SegmentedColumn::Data {
	public:
		friend class SegmentedColumn;

		std::vector<ColumnSegment> segments;
	};

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
SegmentedColumn::SegmentedColumn() {
} //SegmentedColumn::SegmentedColumn


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
SegmentedColumn::SegmentedColumn(const SegmentedColumn& source) : base{ source } {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //SegmentedColumn::SegmentedColumn


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
SegmentedColumn::~SegmentedColumn() {}


/*--------------------------------------------------------------------
	Get the number of segments
 
	return. The number of segments (0 on error)
  --------------------------------------------------------------------*/
size_t SegmentedColumn::getSegmentCount() const {
	confirmPart(getPartFilter());
	return m_data ? m_data->segments.size() : 0;
} //SegmentedColumn::getSegmentCount


/*--------------------------------------------------------------------
	Get a column segment
 
	index: The index of the required segment
 
	return: The requested segment, nullptr on error
  --------------------------------------------------------------------*/
ColumnSegment* SegmentedColumn::getSegment(size_t index) const {
	confirmPart(getPartFilter());
	return (m_data && (index < m_data->segments.size())) ? &m_data->segments[index] : nullptr;
} //SegmentedColumn::getSegment


/*--------------------------------------------------------------------
	Return the bits for the part filter required to load the data necessary to build this object
 
	return: The required filter bits
  --------------------------------------------------------------------*/
Part::filter_bits SegmentedColumn::getPartFilter() const {
#ifdef ARCHICAD
	return APIMemoMask_ColumnSegment | APIMemoMask_AssemblySegmentCut | APIMemoMask_AssemblySegmentScheme | APIMemoMask_AssemblySegmentProfile;
#endif
} //SegmentedColumn::getPartFilter


/*--------------------------------------------------------------------
	Determine if the element memo content has been validated (elements must override according to requirements)
 
	return: True if the element memo content is valid
  --------------------------------------------------------------------*/
bool SegmentedColumn::isPartValid() const {
	return m_data && !m_data->segments.empty();
} //SegmentedColumn::isPartValid


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void SegmentedColumn::loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const {
	
} //SegmentedColumn::loadMemo


/*--------------------------------------------------------------------
	Send the element part back to a memo structure for storage (elements must override according to requirements)
 
	memo: The memo to carry the data
 
	return: True if the data was successfully sent
  --------------------------------------------------------------------*/
bool SegmentedColumn::send(Memo* memo) const {
		//TODO: Complete when required
	return false;
} //SegmentedColumn::send


/*--------------------------------------------------------------------
	Receive the element memo data from a memo structure (elements must override according to requirements)
 
	memo: The memo carrying the data
 
	return: True if the data was successfully received
  --------------------------------------------------------------------*/
bool SegmentedColumn::receive(const Memo& memo) const {
#ifdef ARCHICAD
	if (!memo || (memo.root()->columnSegments == nullptr))
		return false;
	if (m_data)
		m_data->segments.clear();
	else
		m_data = std::make_unique<Data>();
		//Confirm that required data is present in the memo
	auto segmentPtr = memo.root()->columnSegments;
	auto cutPtr = memo.root()->assemblySegmentCuts;
	auto schemePtr = memo.root()->assemblySegmentSchemes;
	auto profilePtr = memo.root()->assemblySegmentProfiles;
	if ((segmentPtr == nullptr) || (cutPtr == nullptr) || (schemePtr == nullptr))
		return false;
		//Determine available item count
	auto segmentCount = BIMMemory::getPtrSize(segmentPtr) / sizeof(API_ColumnSegmentType);
	auto cutCount = BIMMemory::getPtrSize(cutPtr) / sizeof(API_AssemblySegmentCutData);
	auto schemeCount = BIMMemory::getPtrSize(schemePtr) / sizeof(API_AssemblySegmentSchemeData);
	auto profileCount = BIMMemory::getPtrSize(profilePtr) / sizeof(API_AssemblySegmentProfileData);
	if ((segmentCount == 0) || (cutCount != (segmentCount + 1)) || (schemeCount != segmentCount))
		return false;
	auto path = dynamic_cast<const assembly::Path*>(this);
	for (size_t n = 0 ; n < segmentCount; ++n) {
		const API_AssemblySegmentProfileData* thisProfile = nullptr;
		for (size_t i = 0; i < profileCount; ++i) {
			if (profilePtr[i].segmentIndex == n) {
				thisProfile = profilePtr + i;
				break;
			}
		}
		m_data->segments.emplace_back(ColumnSegment{segmentPtr[n], getTableID(), cutPtr[n], cutPtr[n + 1], schemePtr[n], thisProfile});
		m_data->segments.back().setPath(path);
	}
	setMemoLoaded(true);
#endif
	return true;
} //SegmentedColumn::receive
