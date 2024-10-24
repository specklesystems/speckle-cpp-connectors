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


BIMLinkList::BIMLinkList(const ElementIDList& elementIDList) {
	for (const auto& id : elementIDList)
		push_back(id);
}
#endif


