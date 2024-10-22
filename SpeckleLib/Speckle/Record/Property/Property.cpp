#include "Speckle/Record/Property/Property.h"

#include "Speckle/Database/BIMPropertyDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Property/Setting.h"
#include "Speckle/Record/Property/Template.h"
#include "Speckle/Record/Property/Value.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::property;
using namespace speckle::utility;

#include <array>
#include <memory>

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
 Property::Property() {
 
 } //Property::Property


/*--------------------------------------------------------------------
	Constructor
 
	propTemplate: The property template
  --------------------------------------------------------------------*/
Property::Property(std::shared_ptr<Template> propTemplate) : m_template(propTemplate) {
} //Property::Property


/*--------------------------------------------------------------------
	Constructor
 
	value: A property value
	propTemplate: The property template (NB: may override the specified value if it does not meet the template criteria)
  --------------------------------------------------------------------*/
Property::Property(const Value& value, std::shared_ptr<Template> propTemplate) :
		m_setting{std::make_unique<Setting>(value)}, m_template{propTemplate} {
} //Property::Property


/*--------------------------------------------------------------------
	Constructor
 
	setting: A property setting
	propTemplate: The property template (NB: may override the specified value if it does not meet the template criteria)
  --------------------------------------------------------------------*/
Property::Property(const Setting& setting, std::shared_ptr<Template> propTemplate) :
		m_setting{std::make_unique<Setting>(setting)}, m_template{propTemplate} {
} //Property::Property


/*--------------------------------------------------------------------
	Copy constructor
 
	source: The object to copy
  --------------------------------------------------------------------*/
Property::Property(const Property& source) :
		m_setting{source.m_setting ? std::make_unique<Setting>(*source.m_setting) : nullptr}, m_template{source.m_template} {
} //Property::Property


/*--------------------------------------------------------------------
	Move constructor
 
	source: the property to move
  --------------------------------------------------------------------*/
Property::Property(Property&& source) noexcept : m_setting{std::move(source.m_setting)}, m_template{source.m_template} {
} //Property::Property


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Property::~Property() {}


/*--------------------------------------------------------------------
	Determine if the property has a defined value
 
	return: True if a defined value is found
  --------------------------------------------------------------------*/
bool Property::hasDefinedValue() const {
	return m_setting && m_setting->hasDefinedValue();
} //Property::hasDefinedValue


/*--------------------------------------------------------------------
	Get the property name
 
	return: The property name
  --------------------------------------------------------------------*/
String Property::getName() const {
	return m_template ? m_template->getName() : String{};
} //Property::getName


/*--------------------------------------------------------------------
	Get the property group name. NB: This value is not cached in the object and drequires a database lookup - don't use casually
 
	return: The property gorup name
  --------------------------------------------------------------------*/
String Property::getGroupName() const {
	return m_template ? m_template->getGroupName() : String{};
} //Property::getGroupName


/*--------------------------------------------------------------------
	Get the property value as displayed in the UI
 
	return: The property display value
  --------------------------------------------------------------------*/
String Property::getDisplayValue() const {
	return m_setting ? m_setting->getDisplayValue() : String{};
} //Property::getDisplayValue


/*--------------------------------------------------------------------
	Get the property template ID
 
	return: The property template ID (null if the property isn't linked to a template)
  --------------------------------------------------------------------*/
Guid Property::getTemplateID() const {
	return m_template ? m_template->getBIMID() : Guid{};
} //Property::getTemplateID
