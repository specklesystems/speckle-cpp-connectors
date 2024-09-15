#ifndef CONNECTOR_RECORD_MODEL_CARD
#define CONNECTOR_RECORD_MODEL_CARD

#include "Active/Container/Vector.h"
#include "Active/Serialise/Package/Package.h"
#include "Active/Utility/Cloner.h"
#include "Connector/Record/Model/CardSetting.h"
#include "Speckle/Utility/String.h"
#include "Speckle/Database/Content/Record.h"

namespace connector::record {
	
	/*!
	 A connector send filter
	 */
	class ModelCard : public speckle::database::Record {
	public:

		// MARK: - Types
		
		using base = speckle::database::Record;
			//List of card settings
		using SettingList = active::container::Vector<connector::record::CardSetting>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ModelCard() {}
		/*!
		 Destructor
		 */
		virtual ~ModelCard() {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		virtual ModelCard* clonePtr() const override { return new ModelCard(*this); };
		
		// MARK: - Functions (const)
		
		/*!
		 Get the model ID
		 @return The model ID
		 */
		const speckle::utility::String& getModelID() const { return m_modelID; }
		/*!
		 Get the setting type
		 @return The setting type
		 */
		const speckle::utility::String& getProjectID() const { return m_projectID; }
		/*!
		 Get the setting type
		 @return The setting type
		 */
		const speckle::utility::String& getAccountID() const { return m_accountID; }
		/*!
		 Get the setting type
		 @return The setting type
		 */
		const speckle::utility::String& getServerURL() const { return m_serverURL; }
		/*!
		 Get the card settings
		 @return The card settings
		 */
		const SettingList& getSettings() const { return m_settings; }

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
			///The model ID
		speckle::database::RecordID m_modelID;
			///The project ID
		speckle::database::RecordID m_projectID;
			///The user account ID
		speckle::database::RecordID m_accountID;
			///The server URL
		speckle::utility::String m_serverURL;
			///Settings for the model rendering, e.g. level of detail (LoD)
		SettingList m_settings;
	};

}

#endif	//CONNECTOR_RECORD_MODEL_CARD
