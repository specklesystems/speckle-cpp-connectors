#include "Speckle/Database/Identity/BIMLink.h"

using namespace speckle::database;
using namespace speckle::utility;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	selected: Information about a selected Archicad element
  --------------------------------------------------------------------*/
BIMLink::BIMLink(const API_Neig& selected) : base{Guid{selected.guid}} {
	//More info should be extracted from API_Neig in future (as required) - extract into link settings, e.g. selection target etc
} //Link::Link
#endif


