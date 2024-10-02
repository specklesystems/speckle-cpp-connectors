#include "Connector/Interface/Browser/Bridge/Send/Arg/SendViaBrowserArgs.h"

#include "Connector/Record/Model/ModelCard.h"
#include "Speckle/Record/Credentials/Account.h"

#include <array>

using namespace active::serialise;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::database;
using namespace speckle::record::cred;
using namespace speckle::serialise;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		cardID,
		projID,
		modID,
		tokenID,
		serverID,
		accID,
		messageID,
		sendObjectID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"modelCardId"},
		Identity{"projectId"},
		Identity{"modelId"},
		Identity{"token"},
		Identity{"serverUrl"},
		Identity{"accountId"},
		Identity{"message"},
		Identity{"sendObject"},
	};
	
}

/*--------------------------------------------------------------------
	Constructor
 
	modelCard: The model card to populate into the send info for the browser
	account: The account linked to the send
  --------------------------------------------------------------------*/
SendViaBrowserArgs::SendViaBrowserArgs(const ModelCard& modelCard, const Account& account) :
		modelCardID(modelCard.getID()), projectID(modelCard.getProjectID()), modelID(modelCard.getModelID()), token{account.getToken()},
		serverURL{account.getServerURL()}, accountID{account.getID()} {
	
} //SendViaBrowserArgs::SendViaBrowserArgs


/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SendViaBrowserArgs::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[cardID], cardID, element },
			{ fieldID[projID], projID, element },
			{ fieldID[modID], modID, element },
			{ fieldID[tokenID], tokenID, element },
			{ fieldID[serverID], serverID, element },
			{ fieldID[accID], accID, element },
			{ fieldID[messageID], messageID, element },
			{ fieldID[sendObjectID], sendObjectID, element },
		},
	}.withType(&typeid(SendViaBrowserArgs)));
	return true;
} //SendViaBrowserArgs::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SendViaBrowserArgs::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(SendViaBrowserArgs))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case cardID:
			return std::make_unique<StringWrap>(modelCardID);
		case projID:
			return std::make_unique<StringWrap>(projectID);
		case modID:
			return std::make_unique<StringWrap>(modelID);
		case tokenID:
			return std::make_unique<StringWrap>(token);
		case serverID:
			return std::make_unique<StringWrap>(serverURL);
		case accID:
			return std::make_unique<StringWrap>(accountID);
		case messageID:
			return std::make_unique<StringWrap>(message);
		case sendObjectID:
			return std::make_unique<PackageWrap>(sendObject);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendViaBrowserArgs::getCargo
