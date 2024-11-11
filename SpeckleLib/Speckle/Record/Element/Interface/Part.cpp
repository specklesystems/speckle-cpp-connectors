#include "Speckle/Record/Element/Interface/Part.h"

#include "Speckle/Record/Element/Memo.h"

using namespace active::serialise;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Confirm that the element part data is loaded and valid (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
 
	return: True if the part data is ready to use
  --------------------------------------------------------------------*/
bool Part::confirmPart(filter_bits filter) const {
	if (!isMemoLoaded()) {
		std::unique_ptr<Memo> memo;
		loadMemo(filter, memo);
	}
	return isMemoLoaded();
} //Part::confirmPart
