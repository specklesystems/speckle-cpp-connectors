#ifndef SPECKLE_RECORD_PROPERTY
#define SPECKLE_RECORD_PROPERTY

#include "Active/Serialise/Package/Package.h"
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
	class Property : public active::serialise::Package {
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
		 Get the property name
		 @return The property name
		 */
		speckle::utility::String getName() const;
		
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
			///The property setting
		std::unique_ptr<Setting> m_setting;
			///The property template
		std::shared_ptr<Template> m_template;
	};

}

#endif	//SPECKLE_RECORD_PROPERTY
