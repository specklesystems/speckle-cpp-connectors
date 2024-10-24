#ifndef SPECKLE_RECORD_PROPERTY_VALUE
#define SPECKLE_RECORD_PROPERTY_VALUE

#include "Active/Setting/Values/Value.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

namespace speckle::record::property {
	
	/*!
	 Class holding a property value
	 
	 Note that this object holds a bare, single value only, e.g. no metadata. Properties will allow for multiple values and couple them with a
	 property template (defining name etc)
	 Some properties support a key/value pairing, so this object can hold an optional key (undefined when no key is required)
	 NB: This class is not currently intended to be subclassed, but can be revisited in future
	 */
	class Value final {
	public:

		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<Value>;
			///Shared pointer
		using Shared = std::shared_ptr<Value>;
			///Optional
		using Option = std::optional<Value>;

#ifdef ARCHICAD
		/*!
		 Convert an Archicad API property value type to a Value::Type
		 @param type The API property value type
		 @return An equivalent Value::Type
		 */
		static active::setting::Value::Type convert(API_VariantType type);
		/*!
		 Convert Value::Type to an Archicad API property value type
		 @param type The Value::Type
		 @return An equivalent API property value type
		 */
		static API_VariantType convert(active::setting::Value::Type type);
#endif
		
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Value() {}
		/*!
		 Constructor
		 @param value The property value
		 */
		Value(const active::setting::Value& value) : m_value{clone(value)} {}
		/*!
		 Constructor
		 @param value The property value
		 */
		Value(const active::setting::Value& key, const active::setting::Value& value) : m_key{clone(key)}, m_value{clone(value)} {}
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 */
		Value(const API_SingleEnumerationVariant& source, API_VariantStatus status = API_VariantStatusNormal,
			  API_PropertyMeasureType measure = API_PropertyDefaultMeasureType) { receive(source, status, measure); }
		/*!
		 Constructor
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 */
		Value(const API_Variant& source, API_VariantStatus status = API_VariantStatusNormal,
			  API_PropertyMeasureType measure = API_PropertyDefaultMeasureType) { receive(source, status, measure); }
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Value(const Value& source) : m_value{source.m_value ? clone(*source.m_value) : nullptr}, m_key{source.m_key ? clone(*source.m_key) : nullptr} {}

		// MARK: - Operators
			
		/*!
		 Assignment operator
		 @param source The object to copy
		 @return A reference to this
		 */
		Value& operator=(const Value& source);


		// MARK: - Functions (const)

		/*!
		 Determine if the value is defined (and good)
		 @return True if the value is defined
		 */
		bool isDefined() const { return m_value && (m_value->status == active::setting::Value::good); }
		/*!
		 Determine if the value is null, e.g. zero, empty, undefined
		 @return True if the value is null
		 */
		virtual bool isNull() const { return !m_value || m_value->isNull(); }
		/*!
		 Get the value as displayed in the UI
		 @return The value as displayed
		 */
		speckle::utility::String getDisplayValue() const;
		/*!
		 Get the property value key
		 @return The property value key (nullptr if undefined)
		 */
		active::setting::Value* getKey() const { return m_key.get(); }
		/*!
		 Get the property value
		 @return The property value (nullptr if undefined)
		 */
		active::setting::Value* getValue() const { return m_value.get(); }
		
		// MARK: - Functions (mutating)

		/*!
		 Receive a value from an Archicad property
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 @return True if the value was successfully received
		 */
		bool receive(const API_SingleEnumerationVariant& source, API_VariantStatus status = API_VariantStatusNormal,
					 API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
		/*!
		 Receive a value from an Archicad property
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 @return True if the value was successfully received
		 */
		bool receive(const API_Variant& source, API_VariantStatus status = API_VariantStatusNormal,
					 API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
		
	private:
			///The value key
		active::setting::Value::Unique m_key;
			///The property value
		active::setting::Value::Unique m_value;
	};

}

#endif	//SPECKLE_RECORD_PROPERTY_VALUE
