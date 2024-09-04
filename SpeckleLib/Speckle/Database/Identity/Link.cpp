#include "Speckle/Database/Identity/Link.h"
#include "Speckle/Utility/Guid.h"

using namespace speckle::database;
using namespace speckle::utility;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	selected: Information about a selected Archicad element
  --------------------------------------------------------------------*/
Link::Link(const API_Neig& selected) : base{String{Guid{selected.guid}}} {
	//More info should be extracted from API_Neig in future (as required)
} //Link::Link
#endif


