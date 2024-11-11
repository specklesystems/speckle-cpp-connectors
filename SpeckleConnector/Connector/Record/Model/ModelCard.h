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
	 Base class for a Speckle model card

	 A model card captures key information about a model submitted to a Speckle server, e.g. the model ID, the target server
	 and account, and any settings or filters applicable to the host BIM application and open document
	 */
	class ModelCard : public speckle::database::Record {
	public:

		// MARK: - Types
		
		using base = speckle::database::Record;
			///Unique pointer
		using Unique = std::unique_ptr<ModelCard>;
			//List of card settings
		using SettingList = active::container::Vector<connector::record::CardSetting>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ModelCard() {}
		/*!
		 Constructor
		 @param modelID The model ID
		 @param projectID The project ID
		 @param accountID The account ID
		 @param serverURL The server URL
		 @param settings Any BIM host-related settings relevant to the appearance/rendering of the model, e.g. LoD, if properties included etc
		 */
		ModelCard(const speckle::database::RecordID& modelID, const speckle::database::RecordID& projectID,
				  const speckle::database::RecordID& accountID, const speckle::utility::String& serverURL,
				  const SettingList& settings) :
				m_modelID(modelID), m_projectID(projectID), m_accountID(accountID), m_serverURL(serverURL), m_settings(settings) {}
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
		 Get the ID of the active BIM project
		 @return The ID of the active BIM project
		 */
		const speckle::utility::String& getProjectID() const { return m_projectID; }
		/*!
		 Get the user account ID
		 @return The user account ID
		 */
		const speckle::utility::String& getAccountID() const { return m_accountID; }
		/*!
		 Get the user commit message
		 @return The user commit message
		 */
		const speckle::utility::String& getMessage() const { return m_message; }
		/*!
		 Get the server URL
		 @return The server URL
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
			///The commit message from the user for the card
		speckle::utility::String m_message;
			///Settings for the model rendering, e.g. level of detail (LoD)
		SettingList m_settings;
			///True if the card has expired
		bool m_isExpired = false;
	};

}

#endif	//CONNECTOR_RECORD_MODEL_CARD
