#ifndef CONNECTOR_INTERFACE_BRIDGE_SEND_FILTER
#define CONNECTOR_INTERFACE_BRIDGE_SEND_FILTER

#include "Active/Serialise/Package/Package.h"
#include "Active/Utility/Cloner.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 A connector send filter
	 */
	class SendFilter : public active::serialise::Package, public active::utility::Cloner {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param nm The filter name
		 @param sum A summary
		 @param isDef True if this is the default filter
		 */
		SendFilter(const speckle::utility::String& nm = {}, const speckle::utility::String& sum = {}, bool isDef = false) :
				name{nm}, summary{sum}, isDefault{isDef} {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		virtual SendFilter* clonePtr() const override { return new SendFilter(*this); };
		
		// MARK: - Public variables (NB: Assuming to class invariants or overrides for this data, so making public for simplicity)
		
			///The filter name
		speckle::utility::String name;
			///A summary
		speckle::utility::String summary;
			///True if this is the default filter
		bool isDefault = false;

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

#endif	//CONNECTOR_INTERFACE_BRIDGE_SEND_FILTER
