#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT
#define CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT

#include "Active/Serialise/CargoHold.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"
#include "Speckle/Database/Content/Record.h"
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
		 @param object The object to send
		 */
		SendObject(std::unique_ptr<speckle::database::Record> object) : m_object{std::move(object)} { id = m_object->getID(); }
				
		// MARK: - Public variables
		
			///The root object id which should be used for creating the version
		speckle::utility::String id;

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
		void useManagement(active::serialise::Management* management) const override { m_object->useManagement(management); }
		/*!
			Get the cargo management
			@return The active management
		*/
		active::serialise::Management* management() const override { return m_object->management(); }
		
	private:
			///The object to send
		std::unique_ptr<speckle::database::Record> m_object;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_OBJECT
