#ifndef SPECKLE_RECORD_ELEMENT_ASSEMBLY_SEGMENT
#define SPECKLE_RECORD_ELEMENT_ASSEMBLY_SEGMENT

#include "Speckle/Database/Identity/BIMIndex.h"

#include <memory>
#include <optional>

#ifdef ARCHICAD
struct API_AssemblySegmentCutData;
struct API_AssemblySegmentSchemeData;
class ProfileVectorImage;
#endif

namespace active::geometry {
	class PolyEdge;
}

namespace speckle::record::element::assembly {
	
	class Path;
	
	/*!
	 Interface for elements forming a path made up of a consecutive series of segments, e.g. a portal frame made from a series of beams
	 */
	class Segment {
	public:
		/*!
		 Default constructor
		 */
		Segment();
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Segment(const Segment& source);
		/*!
		 Destructor
		 */
		~Segment();
		
		// MARK: - Functions (const)

		/*!
		 Get the segment path
		 @return The segment path (nullptr = undefined)
		 */
		const assembly::Path* getPath() const { return m_path; }
		
		// MARK: - Functions (mutating)

		/*!
		 Set the segment path
		 @param path The segment path
		 */
		void setPath(const assembly::Path* path) { m_path = path; }
		
	protected:
		/*!
		 Constructor
		 @param cutOrigin Cut at the segment origin
		 @param cutEnd Cut at the segment end
		 @param scheme The segment scheme
		 @param profile The segment profile (nullptr = none)
		 */
		Segment(const API_AssemblySegmentCutData& cutOrigin, const API_AssemblySegmentCutData& cutEnd, const API_AssemblySegmentSchemeData& scheme,
				const API_AssemblySegmentProfileData* profile = nullptr);
		
	private:
#ifdef ARCHICAD
			//NB: The following properties are mutable to support lazy loading
			///Cut at the segment origin
		mutable std::unique_ptr<API_AssemblySegmentCutData> m_cutOrigin;
			///Cut at the segment end
		mutable std::unique_ptr<API_AssemblySegmentCutData> m_cutEnd;
			///The segment scheme
		mutable std::unique_ptr<API_AssemblySegmentSchemeData> m_scheme;
			///An optional custom profile (nullptr = none)
		mutable std::unique_ptr<ProfileVectorImage> m_customProfile;
			///An optional stretched profile (nullptr = none)
		mutable std::unique_ptr<ProfileVectorImage> m_stretchedProfile;
#endif
			//The segment edge
		mutable std::unique_ptr<active::geometry::PolyEdge> m_edge;
			//The segment path
		mutable const assembly::Path* m_path = nullptr;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_ASSEMBLY_SEGMENT
