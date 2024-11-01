#ifndef SPECKLE_RECORD_ELEMENT_CONVERSION
#define SPECKLE_RECORD_ELEMENT_CONVERSION

#include "Speckle/Record/Element/ModelElement.h"

#ifdef ARCHICAD
#include "APIdefs_Elements.h"
#endif

namespace speckle::record::element {

#ifdef ARCHICAD
	/*!
	 Convert an Archicad structure type to an element composition
	 */
	ModelElement::Composition convert(API_ModelElemStructureType structType) {
		switch (structType) {
			case API_CompositeStructure:
				return ModelElement::Composition::skinned;
			case API_ProfileStructure:
				return ModelElement::Composition::profiled;
			default:
				return ModelElement::Composition::unordered;
		}
	}
#endif
	
}

#endif	//SPECKLE_RECORD_ELEMENT_CONVERSION
