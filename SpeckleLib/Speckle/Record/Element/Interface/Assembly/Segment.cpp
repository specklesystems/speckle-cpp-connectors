#include "Speckle/Record/Element/Interface/Assembly/Segment.h"

#include "Active/Geometry/PolyEdge.h"
#include "Active/Utility/Memory.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#include <APIdefs_Elements.h>
#include <ProfileVectorImage.hpp>
#endif

using namespace active::geometry;
using namespace speckle::record::element::assembly;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Segment::Segment() :
		m_cutOrigin{std::make_unique<API_AssemblySegmentCutData>()},
		m_cutEnd{std::make_unique<API_AssemblySegmentCutData>()},
		m_scheme{std::make_unique<API_AssemblySegmentSchemeData>()} {

} //Segment::Segment

/*--------------------------------------------------------------------
	Constructor
 
	cutOrigin: Cut at the segment origin
	cutEnd: Cut at the segment end
	scheme: The segment scheme
	profile: The segment profile (nullptr = none)
  --------------------------------------------------------------------*/
Segment::Segment(const API_AssemblySegmentCutData& cutOrigin, const API_AssemblySegmentCutData& cutEnd, const API_AssemblySegmentSchemeData& scheme,
				 const API_AssemblySegmentProfileData* profile) :
		m_cutOrigin{std::make_unique<API_AssemblySegmentCutData>(cutOrigin)},
		m_cutEnd{std::make_unique<API_AssemblySegmentCutData>(cutEnd)},
		m_scheme{std::make_unique<API_AssemblySegmentSchemeData>(scheme)} {
	if (profile != nullptr) {
		if (profile->customOrigProfile != nullptr)
			m_customProfile = std::make_unique<ProfileVectorImage>(*profile->customOrigProfile);
		if (profile->stretchedProfile != nullptr)
			m_stretchedProfile = std::make_unique<ProfileVectorImage>(*profile->stretchedProfile);
	}
} //Segment::Segment

/*--------------------------------------------------------------------
	Copy constructor
 
	source: The object to copy
  --------------------------------------------------------------------*/
Segment::Segment(const Segment& source) :
		m_cutOrigin{std::make_unique<API_AssemblySegmentCutData>(*source.m_cutOrigin)},
		m_cutEnd{std::make_unique<API_AssemblySegmentCutData>(*source.m_cutEnd)},
		m_scheme{std::make_unique<API_AssemblySegmentSchemeData>(*source.m_scheme)},
		m_edge{source.m_edge ? std::make_unique<PolyEdge>(*source.m_edge) : nullptr},
		m_customProfile{source.m_customProfile ? std::make_unique<ProfileVectorImage>(*source.m_customProfile) : nullptr},
		m_stretchedProfile{source.m_stretchedProfile ? std::make_unique<ProfileVectorImage>(*source.m_stretchedProfile) : nullptr} {
	m_path = source.m_path;
} //Segment::Segment


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Segment::~Segment() {}


