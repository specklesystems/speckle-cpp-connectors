#ifndef CONNECTOR_RECORD_ARCHICAD_SELECTION_FILTER
#define CONNECTOR_RECORD_ARCHICAD_SELECTION_FILTER

#include "Speckle/Database/Identity/RecordID.h"
#include "Connector/Record/Model/Filter/DirectSelectionSendFilter.h"

namespace connector::record {
	
	/*!
	 A send filter consisting of a list of selected Archicad element IDs
	 */
	class ArchicadSelectionFilter : public DirectSelectionSendFilter {
	public:

		// MARK: - Types
		
		using base = DirectSelectionSendFilter;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param sum A summary
		 @param isDef True if this is the default filter
		 */
		ArchicadSelectionFilter(const speckle::utility::String& sum = {}, bool isDef = false) : base{sum, isDef} {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		ArchicadSelectionFilter* clonePtr() const override { return new ArchicadSelectionFilter(*this); };

		// MARK: - Functions (const)
	};

}

#endif	//CONNECTOR_RECORD_ARCHICAD_SELECTION_FILTER
