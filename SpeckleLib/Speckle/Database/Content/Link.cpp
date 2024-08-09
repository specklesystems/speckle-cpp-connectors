#include "Speckle/Database/Content/Link.h"

using namespace speckle::database;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	selected: Information about a selected Archicad element
  --------------------------------------------------------------------*/
Link::Link(const API_Neig& selected) : recordID{selected.guid} {
	//More info should be extracted from API_Neig in future (as required)
} //Link::Link
#endif


