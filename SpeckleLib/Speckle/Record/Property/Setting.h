#ifndef SPECKLE_RECORD_PROPERTY_SETTING
#define SPECKLE_RECORD_PROPERTY_SETTING

#include "Speckle/Record/Property/Value.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include <ACAPinc.h>
#endif

namespace speckle::record::property {
	
	/*!
	 Class holding a property setting (as attached to an element)
	 
	 A property setting may have multiple values, distinguishing the Value from the Setting class (which may hold multiple Values)
	 */
	class Setting final {
	public:

		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<Setting>;
			///Shared pointer
		using Shared = std::shared_ptr<Setting>;
			///Optional
		using Option = std::optional<Setting>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Setting() {}
		/*!
		 Constructor
		 @param value The property value
		 */
		Setting(const Value& value) : m_values{value} {}
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 */
		Setting(const API_Variant& source, API_VariantStatus status = API_VariantStatusNormal,
				API_PropertyMeasureType measure = API_PropertyDefaultMeasureType) : m_values{Value{source, status, measure}} {}
		/*!
		 Constructor
		 @param source An Archicad property single value
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 */
		Setting(const API_SingleVariant& source, API_VariantStatus status = API_VariantStatusNormal,
				API_PropertyMeasureType measure = API_PropertyDefaultMeasureType) : m_values{Value{source.variant, status, measure}} {}
		/*!
		 Constructor
		 @param source An Archicad property array value
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 */
		Setting(const API_ListVariant& source, API_VariantStatus status = API_VariantStatusNormal,
				API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
		/*!
		 Constructor
		 @param source An Archicad property enumerated single value
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 */
		Setting(const API_SingleEnumerationVariant& source, API_VariantStatus status = API_VariantStatusNormal,
				API_PropertyMeasureType measure = API_PropertyDefaultMeasureType) : m_values{Value{source, status, measure}} {}
#endif


		// MARK: - Functions (const)

		/*!
		 Determine if the setting is empty, i.e. has no values
		 @return True if the setting is empty
		 */
		bool empty() const { return m_values.empty(); }
		/*!
		 Determine if the setting has a defined value
		 @return True if a defined value is found
		 */
		bool hasDefinedValue() const;
		/*!
		 Determine if the setting is null, e.g. zero, empty, undefined
		 @return True if the setting is null
		 */
		bool isNull() const;
		/*!
		 Get the setting value as displayed in the UI
		 @return The setting display value
		 */
		speckle::utility::String getDisplayValue() const;
		
		// MARK: - Functions (mutating)

		/*!
		 Append a value to the setting
		 @param value The value to append
		 */
		void append(Value&& value);
		
#ifdef ARCHICAD
		/*!
		 Receive a value from an Archicad property
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 @return True if the property data was successfully received
		 */
		bool receive(const API_Variant& source, API_VariantStatus status = API_VariantStatusNormal,
					 API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
		/*!
		 Receive a value from an Archicad property
		 @param source An Archicad property single value
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 @return True if the property data was successfully received
		 */
		bool receive(const API_SingleVariant& source, API_VariantStatus status = API_VariantStatusNormal,
					 API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
		/*!
		 Receive a list of values from an Archicad property
		 @param source An Archicad property array value
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 @return True if the property data was successfully received
		 */
		bool receive(const API_ListVariant& source, API_VariantStatus status = API_VariantStatusNormal,
					 API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
		/*!
		 Receive an enumnerated value from an Archicad property
		 @param source An Archicad property
		 @param status The property status
		 @param measure An optional measurement type, e.g. length, angle (where applicable)
		 @return True if the value was successfully received
		 */
		bool receive(const API_SingleEnumerationVariant& source, API_VariantStatus status = API_VariantStatusNormal,
					 API_PropertyMeasureType measure = API_PropertyDefaultMeasureType);
#endif
		
	private:
			///The property values
		std::vector<Value> m_values;
	};

}

#endif	//SPECKLE_RECORD_PROPERTY_SETTING
