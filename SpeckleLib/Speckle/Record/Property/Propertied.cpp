#include "Speckle/Record/Property/Propertied.h"

#include "Speckle/Database/BIMPropertyDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Classification/Classified.h"
#include "Speckle/Record/Property/Setting.h"
#include "Speckle/Utility/BIMMemory.h"

using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::classify;
using namespace speckle::record::property;
using namespace speckle::utility;

/*--------------------------------------------------------------------
	Get the attached properties
 
	return: The attached properties
  --------------------------------------------------------------------*/
const std::vector<Property>& Propertied::getProperties() const {
	rebuild();
	return *m_properties;
} //Propertied


/*--------------------------------------------------------------------
	Get the number of attached properties
 
	return: The property count
  --------------------------------------------------------------------*/
Propertied::size_type Propertied::getPropertyCount() const {
	rebuild();
	return m_properties->size();
} //Propertied::getPropertyCount


/*--------------------------------------------------------------------
	Get the number of attached properties passing a specified filter
 
	filter: The property filter
 
	return: The number of attached properties passing the filter
  --------------------------------------------------------------------*/
Propertied::size_type Propertied::getPropertyCount(const Filter& filter) const {
	rebuild();
	size_type result = 0;
	for (const auto& property : *m_properties)
		if (filter(property))
			++result;
	return result;
} //Propertied::getPropertyCount


/*--------------------------------------------------------------------
	Get a specified attached property
 
	index: The index of the target property
 
	return: The requested property
  --------------------------------------------------------------------*/
const Property& Propertied::getProperty(size_type index) const {
	rebuild();
	return m_properties->at(index);
} //Propertied::getPropertyCount


/*--------------------------------------------------------------------
	Get a specified attached property using a filter
 
	filter: The property filter
	index: The index of the target property (counting only properties passing the filter)
 
	return: The requested property
  --------------------------------------------------------------------*/
const Property& Propertied::getProperty(const Filter& filter, size_type index) const {
	rebuild();
	for (auto n = 0; n < m_properties->size(); ++n) {
		if (filter((*m_properties)[n])) {
			if (index == 0)
				return (*m_properties)[n];
			--index;
		}
	}
	throw std::out_of_range("Out of bounds property access");
} //Propertied::getPropertyCount


/*--------------------------------------------------------------------
	Rebuild the list of properties
 
	return: True if any properties were found
  --------------------------------------------------------------------*/
bool Propertied::rebuild() const {
	if (m_properties)
		return !m_properties->empty();
	m_properties = std::make_unique<std::vector<Property>>();
		//Ensure this is a BIM record (only BIM records can potentially have attached properties)
	auto record = dynamic_cast<const BIMRecord*>(this);
	if (record == nullptr)
		return false;
	auto project = addon()->getActiveProject().lock();
	if (!project)
		return false;
		//Establish that this record can be classified (and therefore bind to properties - NB: this logic may vary for other BIM platforms)
	auto classified = dynamic_cast<const Classified*>(this);
	if (classified == nullptr)
		return false;
		//Get the record classifications - an empty set means no properties are attached
	auto classificationIDs{classified->getClassificationIDs()};
	if (classificationIDs.empty())
		return false;
		//Get properties linked to the record classifications
	auto propertyDbase = project->getPropertyDatabase();
	auto templates = propertyDbase->findTemplatesByClassification(classificationIDs);
	if (templates.empty())
		return false;
#ifdef ARCHICAD
	GS::Array<API_Guid> propertyIDs;
	for (const auto& propTemplate : templates)
		propertyIDs.Push(propTemplate->getBIMID());
		//Get property values for this record based on the linked properties
	GS::Array<API_Property> properties;
	if (ACAPI_Element_GetPropertyValuesByGuid(record->getBIMID(), propertyIDs, properties) != NoError)
		return false;
	for (const auto& property : properties) {
		if (property.status != API_Property_HasValue)
			continue;
		if (auto iter = std::find_if(templates.begin(), templates.end(),
				[&property](const auto& propTemplate){
					return propTemplate->getBIMID() == Guid{property.definition.guid};
				}); iter != templates.end()) {
			Setting setting;
			if ((**iter).convert(property.value, setting))
				m_properties->emplace_back(Property{setting, *iter});
		}
	}
#endif
	return true;
} //Propertied::rebuild
