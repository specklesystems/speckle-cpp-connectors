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

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		nameID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"name"},
	};

}

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
	Get the property name
 
	return: The property name
  --------------------------------------------------------------------*/
String Property::getName() const {
	return m_template ? m_template->getName() : String{};
} //Property::getName


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Property::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[nameID], nameID, element },
		},
	}.withType(&typeid(Property)));
	return true;
} //Property::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Property::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(Property))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case nameID:
			
		default:
			return nullptr;	//Requested an unknown index
	}
} //Property::getCargo
