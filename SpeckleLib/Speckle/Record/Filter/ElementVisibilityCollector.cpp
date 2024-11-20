#include "Speckle/Record/Filter/ElementVisibilityCollector.h"

using namespace speckle::record;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Apply the filter to a specified element
 
	element: The target element
 
	return: True if the element passed the filter. NB: Always false in this case - its role is data collection using a filter interface
  --------------------------------------------------------------------*/
bool ElementVisibilityCollector::operator()(const speckle::record::element::Element& element) const {
	auto layer = element.getLayer();
	if (layer)
		m_layers.insert({layer->getBIMID(), *layer});
	auto storey = element.getStorey();
	if (storey)
		m_storeys.insert({storey->getBIMID(), *storey});
	return false;
} //ElementVisibilityCollector::operator()
