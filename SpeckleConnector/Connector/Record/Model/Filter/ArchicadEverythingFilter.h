#ifndef CONNECTOR_RECORD_ARCHICAD_EVERYTHING_FILTER
#define CONNECTOR_RECORD_ARCHICAD_EVERYTHING_FILTER

#include "Speckle/Database/Identity/RecordID.h"
#include "Connector/Record/Model/Filter/EverythingSendFilter.h"

namespace connector::record {
	
	/*!
	 A send filter consisting of a list of selected Archicad element IDs
	 */
	class ArchicadEverythingFilter : public EverythingSendFilter {
	public:

		// MARK: - Types
		
		using base = EverythingSendFilter;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param sum A summary
		 @param isDef True if this is the default filter
		 */
		ArchicadEverythingFilter(const speckle::utility::String& sum = {}, bool isDef = false) : base{sum, isDef} {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		ArchicadEverythingFilter* clonePtr() const override { return new ArchicadEverythingFilter(*this); };

		// MARK: - Functions (const)
	};

}

#endif	//CONNECTOR_RECORD_ARCHICAD_EVERYTHING_FILTER
