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


/*--------------------------------------------------------------------
	Determine if the setting has a defined value
 
	return: True if a defined value is found
  --------------------------------------------------------------------*/
bool Setting::hasDefinedValue() const {
	for (const auto& value : m_values)
		if (value.isDefined())
			return true;
	return false;
} //Setting::hasDefinedValue


/*--------------------------------------------------------------------
	Determine if the setting is null, e.g. zero, empty, undefined
 
	return: True if the setting is null
  --------------------------------------------------------------------*/
bool Setting::isNull() const {
	for (const auto& value : m_values)
		if (!value.isNull())
			return false;
	return true;
} //Setting::isNull


/*--------------------------------------------------------------------
	Get the setting value as displayed in the UI
 
	return: The setting display value
  --------------------------------------------------------------------*/
String Setting::getDisplayValue() const {
	String result;
	bool isFirst = true;
	for (const auto& value : m_values) {
		if (value.isDefined()) {
			if (isFirst)
				isFirst = false;
			else
				result += "; ";	//Archicad separates value with a semi-colon - might need to revisit this in different contexts
			result += value.getDisplayValue();
		}
	}
	return result;
} //Setting::getDisplayValue


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
