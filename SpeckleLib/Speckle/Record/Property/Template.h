#ifndef SPECKLE_RECORD_PROPERTY_TEMPLATE
#define SPECKLE_RECORD_PROPERTY_TEMPLATE

#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Record/Property/Value.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::property {
	
	class Group;
	class Setting;
	
	/*!
	 Class defining the characteristics of a property template
	 
	 Properties carry both a template (the property metadata) and a value. The template defines the value characteristics, e.g. name, type etc
	 and can be shared amongst any number of properties
	 Property templates are persisted in the BIM property database
	 */
	class Template : public speckle::database::BIMRecord {
	public:

		// MARK: - Types
		
		using base = speckle::database::BIMRecord;
			///Unique pointer
		using Unique = std::unique_ptr<Template>;
			///Shared pointer
		using Shared = std::shared_ptr<Template>;
			///Optional
		using Option = std::optional<Template>;
			///Template origin, e.g. built-in or custom
		enum class Origin {
			builtInStatic,	///<A built-in static property template
			builtInDynamic,	///<A built-in dynamic property template
			custom,	///<A custom (user-defined) template
		};
			///Template data type
		enum class Type {
			undefined,	///<No data
			single,	///<The property contains a single value
			array,	///<The property contains an array of values
			singleEnum,	///<The property contains a single value from an enumeration og permitted values
			arrayEnum,	///<The property contains an array of values from an enumeration of permitted values
		};
			///Template value measurement type
		enum class Measure {
			none,	///<No measurement type
			length,	///<Length measure
			area,	///<Area measure
			volume,	///<Volumne measure
			angle,	///<Angle measure
		};
		
		// MARK: - Static functions
		
#ifdef ARCHICAD
		/*!
		 Convert an Archicad API measure type to a Measure
		 @param type The API measure type
		 @return An equivalent Measure
		 */
		static Measure convert(API_PropertyMeasureType type);
		/*!
		 Convert Measure to an Archicad API property value type
		 @param type The Measure
		 @return An equivalent API measure type
		 */
		static API_PropertyMeasureType convert(Measure type);
#endif
		
		// MARK: - Constants
		
			///Identifier for a property template table
		inline static utility::Guid propertyTemplateTableID{utility::String{"ae66bc4a-9530-45c9-af57-628562a0d783"}};

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Template();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param source An Archicad property definition to copy
		 */
		Template(const API_PropertyDefinition& source);
#endif
		/*!
		 Constructor
		 @param ID The template ID
		 */
		Template(const database::BIMRecordID& ID);
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 @param unit The record unit type
		 */
		Template(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID,
				std::optional<active::measure::LengthType> unit = active::measure::LengthType::metre);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Template(const Template& source);

		// MARK: - Functions (const)

		/*!
		 Get the template name
		 @return The template name
		 */
		speckle::utility::String getName() const { return m_name; }
		/*!
		 Get the template group ID
		 @return The template group ID
		 */
		speckle::utility::String getGroupID() const { return m_group; }
		/*!
		 Get the template group name. NB: This value is not cached in the object and drequires a database lookup - don't use casually
		 @return The template group name
		 */
		speckle::utility::String getGroupName() const;
		/*!
		 Get the template group. NB: This value is not cached in the object and drequires a database lookup - don't use casually
		 @return The template group (nullptr on failure)
		 */
		std::unique_ptr<Group> getGroup() const;
		/*!
		 Get the classifications linked to the template
		 @return A set containing the IDs of classifications linked to the template
		 */
		const speckle::database::BIMRecordIDList& getClassifications() const { return m_classifications; }
		/*!
		 Determine if the template is linked to a specified classification
		 @param classificationID The ID of the target classification
		 @return True if the template is linked to a specified classification
		 */
		bool linksToClassification(const database::BIMRecordID& classificationID) const { return m_classifications.contains(classificationID); }
		/*!
		 Convert an API_PropertyValue to a Setting. NB: This cannot be done independent of a template (which contains the value characteristics)
		 @param value An Archicad property value
		 @param setting A property setting to receive the property value
		 @return True if the conversion was successful
		 */
		bool convert(const API_PropertyValue& value, Setting& setting) const;
		/*!
		 Convert a setting to an  API_PropertyValue. NB: This cannot be done independent of a template (which contains the value characteristics)
		 @param setting A property setting
		 @param value An Archicad property value to receive the setting
		 @return True if the conversion was successful
		 */
		bool convert(const Setting& setting, API_PropertyValue& value) const;
		/*!
		 Find an enumerated value by key
		 @param key The value key
		 @return The enum value paired with the specified key (nullopt on failure)
		 */
		Value::Option findEnumValue(const Value& key) const;
		
		// MARK: - Functions (mutating)


		
		// MARK: - Serialisation

		/*!
		 Fill an inventory with the package items
		 @param inventory The inventory to receive the package items
		 @return True if the package has added items to the inventory
		 */
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure)
		 */
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;

	private:
			///The template origin (built-in or custom)
		Origin m_origin;
			///The template data type
		Type m_type;
			///The template value type
		active::setting::Value::Type m_valueType;
			///The template data measurement type
		Measure m_measure;
			///Group ID
		speckle::database::BIMRecordID m_group;
			///Name
		speckle::utility::String m_name;
			///Description
		speckle::utility::String m_description;
			///The default value for new property instances
		std::unique_ptr<Setting> m_defaultValue;
			///Expression(s) generating the property value
		std::vector<utility::String> m_expressions;
			///The classifications linked to this template (i.e. elements with any one or these classifications is linked to this template)
		speckle::database::BIMRecordIDList m_classifications;
			///Possible values for enumerated types
		std::vector<Value> m_enumValues;
			///True if the property value is editable
		bool m_isValueEditable;
	};

}

#endif	//SPECKLE_RECORD_PROPERTY_TEMPLATE
