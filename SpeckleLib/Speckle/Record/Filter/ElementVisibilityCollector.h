#ifndef CONNECTOR_ELEMENT_VISIBILTITY_COLLECTOR
#define CONNECTOR_ELEMENT_VISIBILTITY_COLLECTOR

#include "Speckle/Record/Attribute/Layer.h"
#include "Speckle/Record/Attribute/Storey.h"
#include "Speckle/Database/Storage/Element/ElementStorage.h"

namespace speckle::record {
	
	/*!
	 Element layer/storey collector
	 
	 The collector uses a filter interface for easy integration with an element database, but does not actually provide any filtering behaviour
	 */
	class ElementVisibilityCollector : public speckle::database::ElementStorage::Filter {
	public:
		
		// MARK: - Types
		
			///A set of layers
		using Layers = std::unordered_map<database::BIMRecordID, speckle::record::attribute::Layer>;
			///A set of storeys
		using Storeys = std::unordered_map<database::BIMRecordID, speckle::record::attribute::Storey>;
		
		// MARK: - Operators
		
		/*!
		 Apply the filter to a specified element
		 @param element The target element
		 @return True if the element passed the filter. NB: Always false in this case - its role is data collection using a filter interface
		 */
		bool operator()(const speckle::record::element::Element& element) const;
		
		// MARK: - Functions (const)
		
		/*!
		 Get the layers collection from filtered elements
		 @return The collected layers
		 */
		Layers& getLayers() const { return m_layers; }
		/*!
		 Get the storeys collection from filtered elements
		 @return The collected storeys
		 */
		Storeys& getStoreys() const { return m_storeys; }
		
	private:
			///Layers collected from elements
		mutable Layers m_layers;
			///Storeys collected from elements
		mutable Storeys m_storeys;
	};
	
}

#endif //CONNECTOR_ELEMENT_VISIBILTITY_COLLECTOR
