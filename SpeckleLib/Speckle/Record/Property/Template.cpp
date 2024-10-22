#include "Speckle/Record/Property/Template.h"

#include "Speckle/Database/BIMPropertyDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Property/Setting.h"
#include "Speckle/Serialise/Types/Str256.h"

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
	Constructor
 
	ID: The template ID
  --------------------------------------------------------------------*/
Template::Template(const database::BIMRecordID& ID) : base{ID, propertyTableID} {
} //Template::Template


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
Template::Template(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID, std::optional<active::measure::LengthType> unit) :
		base{ID, tableID, unit} {
} //Template::Template


/*--------------------------------------------------------------------
	Copy constructor
 
	source: The object to copy
  --------------------------------------------------------------------*/
Template::Template(const Template& source) : m_origin{source.m_origin}, m_type{source.m_type}, m_valueType{source.m_valueType},
		m_measure{source.m_measure}, m_group{source.m_group}, m_name{source.m_name}, m_description{source.m_description},
		m_defaultValue{source.m_defaultValue ? std::make_unique<Setting>(*source.m_defaultValue) : nullptr}, m_expressions{source.m_expressions},
		m_classifications{source.m_classifications}, m_enumValues{source.m_enumValues}, m_isValueEditable{source.m_isValueEditable} {
} //Template::Template


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Convert an Archicad API measure type to a Measure
 
	type: The API measure type
 
	return: An equivalent Measure
  --------------------------------------------------------------------*/
Template::Measure Template::convert(API_PropertyMeasureType type) {
	switch (type) {
		case API_PropertyLengthMeasureType:
			return Measure::length;
		case API_PropertyAreaMeasureType:
			return Measure::area;
		case API_PropertyVolumeMeasureType:
			return Measure::volume;
		case API_PropertyAngleMeasureType:
			return Measure::angle;
		default:
			break;
	}
	return Measure::none;
} //Template::convert


/*--------------------------------------------------------------------
	Convert Measure to an Archicad API property value type
 
	type: The Measure
 
	return: An equivalent API measure type
  --------------------------------------------------------------------*/
API_PropertyMeasureType Template::convert(Template::Measure type) {
	switch (type) {
		case Measure::length:
			return API_PropertyLengthMeasureType;
		case Measure::area:
			return API_PropertyAreaMeasureType;
		case Measure::volume:
			return API_PropertyVolumeMeasureType;
		case Measure::angle:
			return API_PropertyAngleMeasureType;
		default:
			break;
	}
	return API_PropertyDefaultMeasureType;
} //Template::convert
#endif

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Template::Template() {
} //Template::Template


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	source: An Archicad property definition to copy
  --------------------------------------------------------------------*/
Template::Template(const API_PropertyDefinition& source) : base{source.guid, propertyTableID},
		m_group{source.groupGuid}, m_name(source.name), m_description(source.description) {
	m_origin = static_cast<Origin>(source.definitionType);
	m_type = static_cast<Type>(source.collectionType);
	m_valueType = Value::convert(source.valueType);
	m_measure = Template::convert(source.measureType);
		//Either collect expressions driving the property setting or a default setting
	if (source.defaultValue.hasExpression) {
		for (auto iter = source.defaultValue.propertyExpressions.Begin(); iter != source.defaultValue.propertyExpressions.End(); ++iter)
			m_expressions.push_back(*iter);
	} else {
		m_defaultValue = std::make_unique<Setting>();
		convert(source.defaultValue.basicValue, *m_defaultValue);
	}
	for (const auto& guid : source.availability)
		m_classifications.insert(guid);
	for (const auto& enumVal : source.possibleEnumValues)
		m_enumValues.emplace_back(enumVal);
	m_isValueEditable = source.canValueBeEditable;
} //Template::Template
#endif


/*--------------------------------------------------------------------
	Get the property group name. NB: This value is not cached in the object and drequires a database lookup - don't use casually
 
	return: The property gorup name
  --------------------------------------------------------------------*/
String Template::getGroupName() const {
	String result;
	auto project = addon()->getActiveProject().lock();
	if (project) {
		if (auto group = project->getPropertyDatabase()->getProperty(m_group); group)
			result = group->getName();
	}
	return result;
} //Template::getGroupName


#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Convert an API_PropertyValue to a Setting. NB: This cannot be done independent of a template (which contains the value characteristics)
 
	value: An Archicad property value
	setting: A property setting to receive the property value
 
	return: True if the conversion was successful
  --------------------------------------------------------------------*/
bool Template::convert(const API_PropertyValue& value, Setting& setting) const {
	auto measure = Template::convert(m_measure);
	switch (m_type) {
		case Type::single: case Type::singleEnum:
			return setting.receive(value.singleVariant, API_VariantStatusNormal, measure);
		case Type::array: case Type::arrayEnum:
			return setting.receive(value.listVariant, API_VariantStatusNormal, measure);
		default:
			break;
	}
	return false;
} //Template::convert


/*--------------------------------------------------------------------
	Convert a setting to an  API_PropertyValue. NB: This cannot be done independent of a template (which contains the value characteristics)
 
	setting: A property setting
	value: An Archicad property value to receive the setting
 
	return: True if the conversion was successful
  --------------------------------------------------------------------*/
bool Template::convert(const Setting& setting, API_PropertyValue& value) const {
	return false;	//TODO: Implement when required
} //Template::convert
#endif


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Template::fillInventory(Inventory& inventory) const {
		//Implement when required
	return base::fillInventory(inventory);
} //Template::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Template::getCargo(const Inventory::Item& item) const {
		//Implement when required
	return base::getCargo(item);
} //Template::getCargo
