#include "Speckle/Record/Property/Value.h"

#include "Active/Setting/Values/BoolValue.h"
#include "Active/Setting/Values/GuidValue.h"
#include "Active/Setting/Values/Int64Value.h"
#include "Active/Setting/Values/Measurement/AngleValue.h"
#include "Active/Setting/Values/Measurement/AreaValue.h"
#include "Active/Setting/Values/Measurement/LengthValue.h"
#include "Active/Setting/Values/Measurement/VolumeValue.h"
#include "Active/Setting/Values/NullValue.h"
#include "Active/Setting/Values/StringValue.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

#include <array>

using namespace speckle::record::property;
using namespace speckle::utility;

namespace {
	
		//Factory for values from BIM API properties
#ifdef ARCHICAD
	/*--------------------------------------------------------------------
		Make a value from an Archicad property value

		source: An Archicad property
		status: The property status
		measure: An optional measurement type, e.g. length, angle (where applicable)

		return: A new value copied from the property value
	  --------------------------------------------------------------------*/
	active::setting::Value::Unique makeValue(const API_Variant& source, API_VariantStatus status = API_VariantStatusNormal,
											 API_PropertyMeasureType measure = API_PropertyUndefinedMeasureType) {
		active::setting::Value::Unique result;
			//Create a value according to the source type
		switch (source.type) {
			case API_PropertyIntegerValueType:
				result = std::make_unique<active::setting::Int64Value>(source.intValue);
			case API_PropertyRealValueType: {
					//Ensure measured values are an appropriate type
				switch (measure) {
					case API_PropertyLengthMeasureType:
						result = std::make_unique<active::setting::LengthValue>(source.doubleValue);
					case API_PropertyAreaMeasureType:
						result = std::make_unique<active::setting::AreaValue>(source.doubleValue);
					case API_PropertyVolumeMeasureType:
						result = std::make_unique<active::setting::VolumeValue>(source.doubleValue);
					case API_PropertyAngleMeasureType:
						result = std::make_unique<active::setting::AngleValue>(source.doubleValue);
					default:
						result = std::make_unique<active::setting::DoubleValue>(source.doubleValue);
				}
			}
			case API_PropertyStringValueType:
				result = std::make_unique<active::setting::StringValue>(String{source.uniStringValue});
			case API_PropertyBooleanValueType:
				result = std::make_unique<active::setting::BoolValue>(source.boolValue);
			case API_PropertyGuidValueType:
				result = std::make_unique<active::setting::GuidValue>(Guid{source.guidValue});
			default:
				result = std::make_unique<active::setting::NullValue>();	//TODO: Is this a possible/valid outcome?
		}
			//If the value hasn't already been established as bad, apply the source status
		if (result->status != active::setting::Value::bad) {
			switch (status) {
				case API_VariantStatusNull:
					result->status = active::setting::Value::bad;	//NB: In this context we treat a null value as 'bad'
					break;
				case API_VariantStatusUserUndefined:
					result->status = active::setting::Value::undefined;
					break;
				case API_VariantStatusNormal:
					result->status = active::setting::Value::good;
					break;
			}
		}
		return result;
	} //makeValue
#endif
	
}

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Convert an Archicad API property value type to a Value::Type
 
	type: The API property value type
 
	return: An equivalent Value::Type
  --------------------------------------------------------------------*/
active::setting::Value::Type Value::convert(API_VariantType type) {
	switch (type) {
		case API_PropertyIntegerValueType:
			return active::setting::Value::intType;
		case API_PropertyRealValueType:
			return active::setting::Value::floatType;
		case API_PropertyStringValueType:
			return active::setting::Value::stringType;
		case API_PropertyBooleanValueType:
			return active::setting::Value::boolType;
		case API_PropertyGuidValueType:
			return active::setting::Value::idType;
		default:
			break;
	}
	return active::setting::Value::null;
} //Value::convert


/*--------------------------------------------------------------------
	Convert Value::Type to an Archicad API property value type
 
	type: The Value::Type
 
	return: An equivalent API property value type
  --------------------------------------------------------------------*/
API_VariantType Value::convert(active::setting::Value::Type type) {
	switch (type) {
		case active::setting::Value::intType:
			return API_PropertyIntegerValueType;
		case active::setting::Value::floatType:
			return API_PropertyRealValueType;
		case active::setting::Value::stringType:
			return API_PropertyStringValueType;
		case active::setting::Value::boolType:
			return API_PropertyBooleanValueType;
		case active::setting::Value::idType:
			return API_PropertyGuidValueType;
		default:
			break;
	}
	return API_PropertyUndefinedValueType;
} //Value::convert
#endif


/*--------------------------------------------------------------------
	Assignment operator
 
	source: The object to copy
 
	return: A reference to this
  --------------------------------------------------------------------*/
Value& Value::operator=(const Value& source) {
	if (this != &source) {
		m_value = source.m_value ? clone(*source.m_value) : nullptr;
		m_key = source.m_key ? clone(*source.m_key) : nullptr;
	}
	return *this;
} //Value::operator=


/*--------------------------------------------------------------------
	Get the value as displayed in the UI
 
	return: The value as displayed
  --------------------------------------------------------------------*/
String Value::getDisplayValue() const {
	return m_value ? m_value->operator active::utility::String() : String{};
} //Value::getDisplayValue


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Receive a value from an Archicad property
 
	source: An Archicad property
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
 
	return: True if the value was successfully received
  --------------------------------------------------------------------*/
bool Value::receive(const API_SingleEnumerationVariant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
		//Get the value key (if defined)
	if (source.keyVariant.type != API_PropertyUndefinedValueType) {
		m_key = makeValue(source.keyVariant);
		if (m_key->status != active::setting::Value::good)
			m_key.reset();
	}
		//Get the property value
	m_value = makeValue(source.displayVariant, status, measure);
	return true;
} //Value::receive


/*--------------------------------------------------------------------
	Receive a value from an Archicad property
 
	source: An Archicad property
	status: The property status
	measure: An optional measurement type, e.g. length, angle (where applicable)
 
	return: True if the value was successfully received
  --------------------------------------------------------------------*/
bool Value::receive(const API_Variant& source, API_VariantStatus status, API_PropertyMeasureType measure) {
	m_key.reset();
	m_value = makeValue(source, status, measure);
	return true;
} //Value::receive
#endif
