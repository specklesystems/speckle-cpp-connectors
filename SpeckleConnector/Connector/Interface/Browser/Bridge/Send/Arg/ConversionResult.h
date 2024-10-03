#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_CONVERSION_RESULT
#define CONNECTOR_INTERFACE_BRIDGE_SEND_CONVERSION_RESULT

#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A send error to return to the JS in the event of an error
	 */
	class ConversionResult final : public active::serialise::Package {
	public:
		enum class Status {
			success = 1,
			info,
			warning,
			error,
		};
				
		// MARK: - Constructors
		
		/*!
			Constructor
			@param errMess The error message
			@param card The ID of the model card associated with the wrror
		*/
		ConversionResult(const speckle::utility::String& errMess, const speckle::utility::String& card) : message{errMess}, modelCardID{card} {}
				
		// MARK: - Public variables
		
			///The error message
		speckle::utility::String message;
			///The ID of the model card associated with the data
		speckle::utility::String modelCardID;
			///The element conversion status
		Status status = Status::info;
			///For receive conversion reports, this is the id of the speckle object. For send, it's the host app object id.
		speckle::utility::String sourceId;
			///For receive conversion reports, this is the type of the speckle object. For send, it's the host app object type.
		speckle::utility::String sourceType;
			///For receive conversion reports, this is the id of the host app object. For send, it's the speckle object id.
		speckle::utility::String resultId;
			///For receive conversion reports, this is the type of the host app object. For send, it's the speckle object type.
		speckle::utility::String resultType;
			///The exception (nullopt = no exception)
		SendError::Option error;

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

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_CONVERSION_RESULT
