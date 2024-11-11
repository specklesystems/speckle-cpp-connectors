#include "Speckle/Record/Element/Memo.h"

#include "Active/Utility/Memory.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace speckle::record::element;

namespace speckle::record::element {
	API_Elem_Head Memo::m_dummy{};
}


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Default constructor
 
	memo: A memo structure - this object will take ownership of anything passed to the constructor
  --------------------------------------------------------------------*/
Memo::Memo(std::unique_ptr<API_ElementMemo> memo) : m_data{std::move(memo)} {
	if (!memo) {
			//If no data was supplied, we still allocate an empty in the event that a new element is being constructed
		m_data = std::make_unique<API_ElementMemo>();
		active::utility::Memory::erase(*m_data);
	}
} //Memo::Memo
#endif


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Memo::~Memo() {
#ifdef ARCHICAD
	if (m_data)
		ACAPI_DisposeElemMemoHdls(m_data.get());
#endif
} //Memo::Memo


/*--------------------------------------------------------------------
	Conversion operator
 
	return: True if the memo contains data
  --------------------------------------------------------------------*/
Memo::operator bool() const {
#ifdef ARCHICAD
	return m_data.operator bool();
#else
	return false;
#endif
} //Memo::operator bool
