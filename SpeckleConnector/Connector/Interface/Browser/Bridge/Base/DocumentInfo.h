#ifndef CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_CONFIG

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 Information about the active document, e.g. an Archicad project
	 */
	class DocumentInfo : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param locate The project location
		 @param nm The project name
		 @param guid A unique, persistent ID for the project document
		 */
		DocumentInfo(const speckle::utility::String& locat = {}, const speckle::utility::String& nm = {}, const speckle::utility::String& guid = {}) :
				location{locat}, name{nm}, ID{guid} {}
		
		// MARK: - Public variables (NB: Assuming to class invariants or overrides for this data, so making public for simplicity)
		
			///The project location
		speckle::utility::String location;	//TODO: Confirm this is an address
			///The project name
		speckle::utility::String name;	//TODO: Assume project name rather than document (file) name - need to confirm
			///A unique, persistent ID for the project document
		speckle::utility::String ID;	//TODO: should possibly be a guid - need to check
		
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
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_CONNECTOR_CONFIG
