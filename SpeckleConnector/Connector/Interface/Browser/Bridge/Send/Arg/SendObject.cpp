#include "Connector/Interface/Browser/Bridge/Send/Send.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"
#include "Speckle/Serialise/Detached/Storage/DetachedMemoryStore.h"
#include "Speckle/Utility/Exception.h"

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::database;
using namespace speckle::serialise;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		errorID,
		cardID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"error"},
		Identity{"modelCardId"},
	};

	/*!
	 A send error to return to the JS in the event of an error
	 */
	class SendError final : public active::serialise::Package {
	public:
				
		// MARK: - Constructors
		
		/*!
			Constructor
			@param errMess The error message
			@param card The ID of the model card associated with the wrror
		*/
		SendError(const String& errMess, const String& card) : message{errMess}, modelCardID{card} {}
				
		// MARK: - Public variables
		
			///The error message
		String message;
			///The ID of the model card associated with the data
		String modelCardID;

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
	};
	

	
}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool SendError::fillInventory(active::serialise::Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[errorID], errorID, element },
			{ fieldID[cardID], cardID, element },
		},
	}.withType(&typeid(SendError)));
	return true;
} //SendError::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique SendError::getCargo(const active::serialise::Inventory::Item& item) const {
	if (item.ownerType != &typeid(SendError))
		return nullptr;
	using namespace active::serialise;
	switch (item.index) {
		case errorID:
			return std::make_unique<ValueWrap<String>>(message);
		case cardID:
			return std::make_unique<ValueWrap<String>>(modelCardID);
		default:
			return nullptr;	//Requested an unknown index
	}
} //SendError::getCargo


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Send::Send() : BridgeMethod{"Send", [&](const SendArgs& args) {
		run(args);
}} {}


/*--------------------------------------------------------------------
	Send a specified model
 
	modelCardID: The ID of the model to send
  --------------------------------------------------------------------*/
void Send::run(const String& modelCardID) const {
		//Find the specified model card
	auto modelCardDatabase = connector()->getModelCardDatabase();
	auto modelCard = modelCardDatabase->getCard(modelCardID);
	if (!modelCard) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, modelCardNotFoundID), modelCardID));
		return;
	}
		//Get the active project
	auto project = connector()->getActiveProject().lock();
	if (!project) {
		getBridge()->sendEvent("setModelError",
					std::make_unique<SendError>(connector()->getLocalString(errorString, noProjectOpenID), modelCardID));
		return;
	}
	
	DetachedMemoryStore detachedObjects;
} //Send::run
