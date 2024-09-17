#ifndef CONNECTOR_RECORD_DIRECT_SELECT_SEND_FILTER
#define CONNECTOR_RECORD_DIRECT_SELECT_SEND_FILTER

#include "Connector/Database/Identity/RecordID.h"
#include "Connector/Record/Model/Filter/SendFilter.h"

namespace connector::record {
	
	/*!
	 A send filter consisting of a list of selected element IDs
	 */
	class DirectSelectionSendFilter : public SendFilter {
	public:

		// MARK: - Types
		
		using base = SendFilter;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param sum A summary
		 @param isDef True if this is the default filter
		 */
		DirectSelectionSendFilter(const speckle::utility::String& sum = {}, bool isDef = false) : base{"Selection", sum, isDef} {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		DirectSelectionSendFilter* clonePtr() const override { return new DirectSelectionSendFilter(*this); };

		// MARK: - Functions (const)
		
		/*!
		 Get the filtered element IDs
		 @return The filter elements
		 */
		const database::ElementIDList& getElementIDs() const override { return m_selectedElements; }
		
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
		/*!
			Set to the default package content
		*/
		void setDefault() override;
		
	private:
			///A list of selected element IDs
		database::ElementIDList m_selectedElements;
	};

}

#endif	//CONNECTOR_RECORD_DIRECT_SELECT_SEND_FILTER
