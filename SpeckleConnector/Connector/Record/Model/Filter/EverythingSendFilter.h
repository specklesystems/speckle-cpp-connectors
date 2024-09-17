#ifndef CONNECTOR_RECORD_EVERYTHING_SEND_FILTER
#define CONNECTOR_RECORD_EVERYTHING_SEND_FILTER

#include "Connector/Database/Identity/RecordID.h"
#include "Connector/Record/Model/Filter/SendFilter.h"

namespace connector::record {
	
	/*!
	 A send filter consisting of a list of selected element IDs
	 */
	class EverythingSendFilter : public SendFilter {
	public:

		// MARK: - Types
		
		using base = SendFilter;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param sum A summary
		 @param isDef True if this is the default filter
		 */
		EverythingSendFilter(const speckle::utility::String& sum = {}, bool isDef = false) : base{"Everything", sum, isDef} {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		EverythingSendFilter* clonePtr() const override { return new EverythingSendFilter(*this); };

		// MARK: - Functions (const)
		
		/*!
		 Get the filtered element IDs
		 @return The filter elements
		 */
		const database::ElementIDList& getElementIDs() const override { return m_emptyList; }
		/*!
		 Determine if the filter has expired because an element in the selection has changed
		 @param changed The list of changed element IDs
		 @return True if the one of the changed elements is in the selection
		 */
		virtual bool checkExpiry(const database::ElementIDList& changed) const override { return true; }
		
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
			///Enables a const empty list to be returned
		database::ElementIDList m_emptyList;
	};

}

#endif	//CONNECTOR_RECORD_EVERYTHING_SEND_FILTER
