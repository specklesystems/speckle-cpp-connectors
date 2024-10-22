#ifndef SPECKLE_RECORD_PROPERTY
#define SPECKLE_RECORD_PROPERTY

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

namespace speckle::record::property {
	
	class Setting;
	class Template;
	class Value;

	/*!
	 Class holding a property value attached to a BIM model element
	 
	 Properties are always coupled with a template that defines its characteristics (type, name, units etc). This object primarily holds the
	 property value, coupled with a reference to the template
	 Values are always stored with an optional key value (for enumerated types). For other values, the key will be undefined
	 Note that some properties support multiple values - the property holds all these values individually, but always exports a single as a single
	 value by concatonating values usng the Archicad convention of a separating semi-colon, e.g. "Value1; Value 2". This can be revisited in
	 future if alernative export/display methods should be supported
	 */
	class Property {
	public:

		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<Property>;
			///Shared pointer
		using Shared = std::shared_ptr<Property>;
			///Optional
		using Option = std::optional<Property>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Property();
		/*!
		 Constructor
		 @param propTemplate The property template
		 */
		Property(std::shared_ptr<Template> propTemplate);
		/*!
		 Constructor
		 @param value A property value
		 @param propTemplate The property template (NB: may override the specified value if it does not meet the template criteria)
		 */
		Property(const Value& value, std::shared_ptr<Template> propTemplate);
		/*!
		 Constructor
		 @param setting A property setting
		 @param propTemplate The property template (NB: may override the specified value if it does not meet the template criteria)
		 */
		Property(const Setting& setting, std::shared_ptr<Template> propTemplate);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Property(const Property& source);
		/*!
		 Move constructor
		 @param source the property to move
		 */
		Property(Property&& source) noexcept;
		/*!
		 Destructor
		 */
		~Property();

		// MARK: - Functions (const)

		/*!
		 Determine if the property has a defined value
		 @return True if a defined value is found
		 */
		bool hasDefinedValue() const;
		/*!
		 Get the property name
		 @return The property name
		 */
		speckle::utility::String getName() const;
		/*!
		 Get the property group name. NB: This value is not cached in the object and drequires a database lookup - don't use casually
		 @return The property gorup name
		 */
		speckle::utility::String getGroupName() const;
		/*!
		 Get the property value as displayed in the UI
		 @return The property display value
		 */
		speckle::utility::String getDisplayValue() const;
		/*!
		 Get the property template ID
		 @return The property template ID (null if the property isn't linked to a template)
		 */
		speckle::utility::Guid getTemplateID() const;
		
		// MARK: - Functions (mutating)

	private:
			///The property setting
		std::unique_ptr<Setting> m_setting;
			///The property template
		std::shared_ptr<Template> m_template;
	};

}

#endif	//SPECKLE_RECORD_PROPERTY
