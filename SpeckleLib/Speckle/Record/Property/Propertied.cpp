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
	return true;
} //Propertied::rebuild
