#ifndef SPECKLE_RECORD_ELEMENT_QUANTS_SURVEYOR
#define SPECKLE_RECORD_ELEMENT_QUANTS_SURVEYOR

#include "Speckle/Database/Identity/BIMLink.h"

#ifdef ARCHICAD
#include "APIdefs_Elements.h"
#endif

namespace speckle::record::element::quants {
	
	/*!
	 A utility class to measure element quantities
	 */
	class Surveyor {
	public:
		/*!
		 Default constructor
		 */
		Surveyor();
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Surveyor(const Surveyor& source);
		
		
		/*!
		 Assignment operator
		 @param source The object to copy
		 @return A reference to this
		 */
		Surveyor& operator= (const Surveyor& source);

#ifdef ARCHICAD
		/*!
		 Get the quantity measurement values
		 @return The quantity valuse
		 */
		API_ElementQuantity& quantity()	{ return m_quantity; }
		/*!
		 Get the mask specifying the values to measure
		 @return The quantity mask
		 */
		API_QuantitiesMask& mask()	{ return m_mask; }
		/*!
		 Get the mask specifying the element values to measure
		 @return The element quantity mask
		 */
		API_ElementQuantityMask& elementMask() { return mask().elements; }
		/*!
		 Get the mask specifying the composite values to measure
		 @return The composite quantity mask
		 */
		API_CompositeQuantityMask& compositeMask() { return mask().composites; }
#endif
		/*!
		 Measure quantities from a specified element
		 @param elementLink Link to the target element
		 @return True if measurements were successfully taken
		 */
		bool measure(const database::BIMLink& elementLink);
		
#ifdef ARCHICAD
		/*!
		 Get the quantity measurement values
		 @return The quantity valuse
		 */
		const API_ElementQuantity& quantity() const	{ return m_quantity; }
		/*!
		 Get the mask specifying the values to measure
		 @return The quantity mask
		 */
		const API_QuantitiesMask& mask() const	{ return m_mask; }
		/*!
		 Get the mask specifying the composite values to measure
		 @return The composite quantity mask
		 */
		const GS::Array<API_CompositeQuantity>& composites() const { return m_composites; }
#endif
		
	private:
#ifdef ARCHICAD
			///Archicad quantity measurement values
		API_ElementQuantity m_quantity;
			///Mask specifying the values to measure
		API_QuantitiesMask m_mask;
			///Optional additional values to measure, e.g. composite skins, assembly parts etc
		API_Quantities m_extendedQuantity;
			///Measurements taken from copmposite skins, e.g. the construction layers of a roof/wall
		GS::Array<API_CompositeQuantity> m_composites;
#endif
	};
	
}

#endif //SPECKLE_RECORD_ELEMENT_QUANTS_SURVEYOR
