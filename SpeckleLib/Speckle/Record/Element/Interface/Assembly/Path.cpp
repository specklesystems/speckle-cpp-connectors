#include "Speckle/Record/Element/Interface/Assembly/Path.h"

#include "Active/Geometry/PolyEdge.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

using namespace active::geometry;
using namespace speckle::database;
using namespace speckle::record::element::assembly;

/*--------------------------------------------------------------------
	Get a specified edge from the segmented path
 
	index: The index of the required edge
 
	return: The requested edge (nullptr on failure)
  --------------------------------------------------------------------*/
std::unique_ptr<active::geometry::PolyEdge> Path::getSegmentEdge(const database::BIMIndex& index) const {
		//TODO: Complete when required
	return nullptr;
}
