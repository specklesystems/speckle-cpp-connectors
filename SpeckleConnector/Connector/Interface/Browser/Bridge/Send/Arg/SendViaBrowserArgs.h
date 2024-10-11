#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_VIA_BROWSER_ARGS
#define CONNECTOR_INTERFACE_BRIDGE_SEND_VIA_BROWSER_ARGS

#include "Active/Serialise/Package/Package.h"
#include "Active/Utility/String.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/ConversionResult.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendObject.h"
#include "Speckle/Database/Identity/RecordID.h"

namespace speckle::record::cred {
	class Account;
}

namespace connector::record {
	class ModelCard;
}

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
	/*!
	 A commit of a project version (model) to the Speckle server
	 
	 An object of this type is prepared by the Send bridge method
	 */
	class SendViaBrowserArgs final : public active::serialise::Package {
	public:
				
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param modelCard The model card to populate into the send info for the browser
		 @param account The account linked to the send
		 @param object The object to be sent
		 */
		SendViaBrowserArgs(const connector::record::ModelCard& modelCard, const speckle::record::cred::Account& account, SendObject&& object);
				
		// MARK: - Public variables
		
			///ID of the model card driving the send request
		speckle::database::RecordID modelCardID;
			///The source project ID (from the model card)
		speckle::database::RecordID projectID;
			///The model ID (from the model card)
		speckle::database::RecordID modelID;
			///The account token (from the user account info)
		speckle::utility::String token;
			///The server URL (from the user account info)
		speckle::utility::String serverURL;
			///The user account ID
		speckle::utility::String accountID;
			///The send message
		speckle::utility::String message;	//TODO: Clarify what this is used for
			///The conversion report (summarising the conversion results on an element-by-element basis)
		std::vector<ConversionResult> sendConversionResults;
			///The commit content
		SendObject sendObject;

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
			Use a manager in (de)serialisation processes
			@param management The management to use
		*/
		void useManagement(active::serialise::Management* management) const override { sendObject.useManagement(management); }
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_VIA_BROWSER_ARGS
