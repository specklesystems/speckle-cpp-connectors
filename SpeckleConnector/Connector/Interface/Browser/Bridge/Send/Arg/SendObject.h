#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT
#define CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 Class defining the primary content of a send
	 */
	class SendObject final : public active::serialise::Package {
	public:
				
		// MARK: - Constructors
		
		/*!
			Constructor
			@param errMess The error message
			@param card The ID of the model card associated with the wrror
		*/
		SendObject() {}
				
		// MARK: - Public variables
		
			///The root object id which should be used for creating the version
		speckle::utility::String id = "1234asdasdsadsadsa";
			///The total number of children
		int32_t totalChildrenCount = 0;
			///JSON batches for the root object and child (detached) objects
		std::vector<speckle::utility::String> batches = { "[{\"speckle_type\": \"Base\", \"id\" : \"1234asdasdsadsadsa\"}]" }; // NOTE to Ralph: it will be stringified but like array of objects as in example

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

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT
