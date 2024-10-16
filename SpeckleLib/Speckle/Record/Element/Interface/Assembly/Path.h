#ifndef SPECKLE_RECORD_ELEMENT_ASSEMBLY_PATH
#define SPECKLE_RECORD_ELEMENT_ASSEMBLY_PATH

#include "Speckle/Database/Identity/BIMIndex.h"

#include <memory>
#include <optional>

namespace active::geometry {
	class PolyEdge;
}

namespace speckle::record::element::assembly {
	
	class Segment;
	
	/*!
	 Interface for assemblies forming a path made up of a consecutive series of segments, e.g. a portal frame made from a series of beams
	 */
	class Path {
	public:

		// MARK: - Functions (const)

		/*!
		 Get a specified edge from the segmented path
		 @param index The index of the required edge
		 @return The requested edge (nullptr on failure)
		 */
		std::unique_ptr<active::geometry::PolyEdge> getSegmentEdge(const database::BIMIndex& index) const;
		
	protected:
		/*!
		 Get the number of segments in the path (elements must override according to requirements)
		 @return The segment count
		 */
		virtual size_t getSegmentCount() const = 0;
		/*!
		 Get a segment from the path (elements must override according to requirements)
		 @param index The index of the required segment
		 @return The requested segment (nullptr on failure)
		 */
		virtual Segment* getSegment(size_t index) const = 0;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_ASSEMBLY_PATH
