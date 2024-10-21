#include "Speckle/Record/Property/Setting.h"

#include "Speckle/Utility/String.h"

using namespace speckle::record::property;
using namespace speckle::utility;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	source: An Archicad property array value
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
  --------------------------------------------------------------------*/
Setting::Setting(const API_ListVariant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
	for (const auto& val : source.variants)
		m_values.emplace_back(Value{val, status, measure});
} //Setting::Setting
#endif


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Receive a value from an Archicad property
 
	source: An Archicad property
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
 
	return: True if the property data was successfully received
  --------------------------------------------------------------------*/
bool Setting::receive(const API_Variant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
	m_values.clear();
	m_values.emplace_back(Value{source, status, measure});
	return true;
} //Setting::receive


/*--------------------------------------------------------------------
	Receive a value from an Archicad property
 
	source: An Archicad property single value
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
 
	return: True if the property data was successfully received
  --------------------------------------------------------------------*/
bool Setting::receive(const API_SingleVariant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
	m_values.clear();
	m_values.emplace_back(Value{source.variant, status, measure});
	return true;
} //Setting::receive


/*--------------------------------------------------------------------
	Receive a list of values from an Archicad property
 
	source: An Archicad property array value
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
 
	return: True if the property data was successfully received
  --------------------------------------------------------------------*/
bool Setting::receive(const API_ListVariant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
	m_values.clear();
	for (const auto& val : source.variants)
		m_values.emplace_back(Value{val, status, measure});
	return true;
} //Setting::receive


/*--------------------------------------------------------------------
	Receive an enumnerated value from an Archicad property
 
	source: An Archicad property
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
 
	return: True if the value was successfully received
  --------------------------------------------------------------------*/
bool Setting::receive(const API_SingleEnumerationVariant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
	m_values.clear();
	m_values.emplace_back(Value{source, status, measure});
	return true;
} //Setting::receive
#endif
