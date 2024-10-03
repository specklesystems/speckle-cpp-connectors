#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_ERROR
#define CONNECTOR_INTERFACE_BRIDGE_SEND_ERROR

#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A send error to return to the JS in the event of an error
	 */
	class SendError final : public active::serialise::Package {
	public:
		
		// MARK: Types
		
			///Optional
		using Option = std::optional<SendError>;
				
		// MARK: - Constructors
		
		/*!
			Constructor
			@param errMess The error message
			@param card The ID of the model card associated with the wrror
		*/
		SendError(const speckle::utility::String& errMess, const speckle::utility::String& card) : message{errMess}, modelCardID{card} {}
				
		// MARK: - Public variables
		
			///The error message
		speckle::utility::String message;
			///The ID of the model card associated with the data
		speckle::utility::String modelCardID;

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

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_ERROR
