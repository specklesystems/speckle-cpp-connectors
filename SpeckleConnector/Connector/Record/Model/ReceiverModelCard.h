#ifndef CONNECTOR_RECORD_RECEIVER_MODEL_CARD
#define CONNECTOR_RECORD_RECEIVER_MODEL_CARD

#include "Speckle/Database/Identity/RecordID.h"
#include "Connector/Record/Model/ModelCard.h"

namespace connector::record {
	
	class SendFilter;
	
	/*!
	 A connector model send card - carries information about what was sent with the model
	 */
	class ReceiverModelCard : public connector::record::ModelCard {
	public:

		// MARK: - Types
		
		using base = connector::record::ModelCard;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ReceiverModelCard() {}
		/*!
		 Constructor
		 @param projectName The project name
		 @param modelName The model name
		 @param selectedVersion The selected version ID
		 @param latestVersion The latest version ID
		 @param hasDimissedWarning True if the user has already dismissed an alert to update
		 @param bakedObjects The IDs of objects accepted in the receive
		 */
		ReceiverModelCard(const speckle::database::RecordID& projectID, const speckle::utility::String& projectName,
						  const speckle::database::RecordID& modelID, const speckle::utility::String& modelName,
						  const speckle::database::RecordID& selectedVersion, const speckle::database::RecordID& latestVersion,
						  const speckle::database::RecordID& accountID, const speckle::utility::String& serverURL,
						  bool hasDimissedWarning, speckle::database::ElementIDList&& bakedObjects, const SettingList& settings) :
				ModelCard{modelID, projectID, accountID, serverURL, settings},
				m_projectName{projectName}, m_modelName{modelName}, m_selectedVersionID{selectedVersion}, m_latestVersionID{latestVersion},
				m_hasDismissedUpdateWarning{hasDimissedWarning}, m_bakedObjectIDs{bakedObjects} {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		virtual ReceiverModelCard* clonePtr() const override { return new ReceiverModelCard(*this); };
		
		// MARK: - Functions (const)
		
		/*!
		 Get the project name
		 @return The project name
		 */
		const speckle::utility::String& getProjectName() const { return m_projectName; }
		/*!
		 Get the model name
		 @return The model name
		 */
		const speckle::utility::String& getModelName() const { return m_modelName; }
		/*!
		 Get the selected version ID
		 @return The selected version ID
		 */
		const speckle::database::RecordID& getSelectedVersionID() const { return m_selectedVersionID; }
		/*!
		 Get the latest version ID
		 @return The latest version ID
		 */
		const speckle::database::RecordID& getLatestVersionID() const { return m_latestVersionID; }
		/*!
		 Determine if the user has already dismissed an alert to update
		 @return True if the user has already dismissed an alert to update
		 */
		bool hasDismissedUpdateWarning() const { return m_hasDismissedUpdateWarning; }
		/*!
		 Get the IDs of objects accepted in the receive
		 @return The accepted object IDs
		 */
		const speckle::database::ElementIDList& getBakedObjectIDs() const { return m_bakedObjectIDs; }

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
			///The received project name
		speckle::utility::String m_projectName;
			///The received model name
		speckle::utility::String m_modelName;
			///The ID of the version selected in the receive
		speckle::database::RecordID m_selectedVersionID;
			///The ID of the latest version
		speckle::database::RecordID m_latestVersionID;
			///True if the user has already dismissed an alert to update
		bool m_hasDismissedUpdateWarning = false;
			///IDs of objects accepted in the receive
		speckle::database::ElementIDList m_bakedObjectIDs;
	};

}

#endif	//CONNECTOR_RECORD_RECEIVER_MODEL_CARD
