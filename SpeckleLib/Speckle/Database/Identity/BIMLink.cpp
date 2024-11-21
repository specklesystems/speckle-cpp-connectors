#include "Speckle/Database/Identity/BIMLink.h"

using namespace speckle::database;
using namespace speckle::utility;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	selected: Information about a selected Archicad element
	tableID: The ID of the parent table
  --------------------------------------------------------------------*/
BIMLink::BIMLink(const API_Neig& selected, const BIMRecordID& tableID) : base{Guid{selected.guid}, tableID} {
	//More info should be extracted from API_Neig in future (as required) - extract into link settings, e.g. selection target etc
} //Link::Link
#endif


/*--------------------------------------------------------------------
	Constructor
 
	elementIDList: BIM record IDs to populate the links
  --------------------------------------------------------------------*/
BIMLinkList::BIMLinkList(const BIMRecordIDList& elementIDList) {
	for (const auto& id : elementIDList)
		push_back(id);
} //BIMLinkList::BIMLinkList


/*--------------------------------------------------------------------
	Conversion operator
 
	return: A list of BIM record IDs from this list
  --------------------------------------------------------------------*/
BIMLinkList::operator BIMRecordIDList() const {
	BIMRecordIDList result;
	for (const auto& link : *this)
		result.insert(BIMRecordID{link});
	return result;
} //BIMLinkList::operator BIMRecordIDList
