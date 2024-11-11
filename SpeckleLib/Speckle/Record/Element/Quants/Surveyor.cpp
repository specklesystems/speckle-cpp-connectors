#include "Speckle/Record/Element/Quants/Surveyor.h"

#include "Active/Utility/Memory.h"
#include "Speckle/Record/Element/Setting/TypeSetting.h"

using namespace active::math;
using namespace speckle::database;
using namespace speckle::record::element::quants;

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Surveyor::Surveyor() {
#ifdef ARCHICAD
	active::utility::Memory::erase(m_quantity);
	active::utility::Memory::erase(m_mask);
	m_extendedQuantity.elements = &m_quantity;
	m_extendedQuantity.composites = &m_composites;
#endif
} //Surveyor::Surveyor


/*--------------------------------------------------------------------
	Copy constructor
	
	source: The object to copy
  --------------------------------------------------------------------*/
Surveyor::Surveyor(const Surveyor& source) : m_quantity(source.m_quantity), m_mask(source.m_mask), m_composites{source.m_composites} {
#ifdef ARCHICAD
	m_extendedQuantity.elements = &m_quantity;
	m_extendedQuantity.composites = &m_composites;
#endif
} //Surveyor::Surveyor


/*--------------------------------------------------------------------
	Assignment operator
	
	source: The object to copy
  --------------------------------------------------------------------*/
Surveyor& Surveyor::operator= (const Surveyor& source) {
	if (this != &source) {
#ifdef ARCHICAD
		m_quantity = source.m_quantity;
		m_mask = source.m_mask;
		m_composites = source.m_composites;
		m_extendedQuantity.elements = &m_quantity;
		m_extendedQuantity.composites = &m_composites;
#endif
	}
	return *this;
} //Surveyor::operator=


/*--------------------------------------------------------------------
	Measure quantities from a specified element
 
	elementLink: Link to the target element
 
	return: True if measurements were successfully taken
  --------------------------------------------------------------------*/
bool Surveyor::measure(const BIMLink& elementLink) {
	bool result = false;
#ifdef ARCHICAD
	API_QuantityPar quantParams;
	active::utility::Memory::erase(quantParams);
	active::utility::Memory::erase(m_quantity);
	GS::Array<API_ElemPartQuantity> elemPartQuantities;
	bool isPartQuantities = false;
	if (auto typeSetting = elementLink.find<TypeSetting>(TypeSetting::ID); typeSetting != nullptr)
		isPartQuantities = (typeSetting->type == API_MorphID);
	m_extendedQuantity.elemPartQuantities = isPartQuantities ? &elemPartQuantities : nullptr;
	quantParams.minOpeningSize = eps;
	m_composites.Clear();
	result = (ACAPI_Element_GetQuantities(elementLink, &quantParams, &m_extendedQuantity, &m_mask) == NoError);
	if (isPartQuantities) {
		if (result && (elemPartQuantities.GetSize() > 0)) {
			m_quantity.morph.baseLevel = elemPartQuantities[0].quantity.morph.baseLevel;
			m_quantity.morph.baseHeight = elemPartQuantities[0].quantity.morph.baseHeight;
			m_quantity.morph.wholeHeight = elemPartQuantities[0].quantity.morph.wholeHeight;
		}
		m_extendedQuantity.elemPartQuantities = nullptr;
	}
#endif
	return result;
} //Surveyor::measure
