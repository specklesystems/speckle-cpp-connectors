#ifndef CONNECTOR_RECORD_SENDER_MODEL_CARD
#define CONNECTOR_RECORD_SENDER_MODEL_CARD

#include "Connector/Record/Model/ModelCard.h"

namespace connector::record {
	
	class SendFilter;
	
	/*!
	 A connector model send card - carries information about what was sent with the model
	 */
	class SenderModelCard : public connector::record::ModelCard {
	public:

		// MARK: - Types
		
		using base = connector::record::ModelCard;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SenderModelCard();
		/*!
		 Constructor
		 @param filter The filter applied when the model was sent
		 */
		SenderModelCard(const SendFilter& filter);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		SenderModelCard(const SenderModelCard& source);
		/*!
		 Destructor
		 */
		~SenderModelCard();
		/*!
			Record cloning
			@return A clone of this record
		*/
		virtual SenderModelCard* clonePtr() const override { return new SenderModelCard(*this); };
		
		// MARK: - Functions (const)
		
		/*!
		 Get the filter applied when the model was sent
		 @return The model filter
		 */
		const SendFilter& getFilter() const { return *m_filter; }

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
			///The filter applied when the model was sent
		std::unique_ptr<SendFilter> m_filter;
	};

}

#endif	//CONNECTOR_RECORD_SENDER_MODEL_CARD
