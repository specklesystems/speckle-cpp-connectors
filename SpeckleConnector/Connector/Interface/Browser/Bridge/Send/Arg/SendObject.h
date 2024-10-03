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
		
		using base = std::reference_wrapper<active::serialise::Package>;
				
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param objID The ID of the object to send
		 @param object The object to send
		 */
		SendObject(const speckle::utility::String& objID, std::unique_ptr<active::serialise::Package> object) : m_object{std::move(object)} {}
				
		// MARK: - Public variables
		
			///The root object id which should be used for creating the version
		speckle::utility::String id = "1234";

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
		
	private:
			///The object to send
		std::unique_ptr<active::serialise::Package> m_object;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT
