#ifndef CONNECTOR_INTERFACE_BRIDGE_ERROR_REPORT
#define CONNECTOR_INTERFACE_BRIDGE_ERROR_REPORT

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace speckle::interfac::browser::bridge {
	
	/*!
	 Information about a runtime error, typically extracted from an exception
	 */
	class ErrorReport : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param mess The project location
		 @param err The project name
		 @param trace A unique, persistent ID for the project document
		 */
		ErrorReport(const speckle::utility::String& mess = {}, const speckle::utility::String& err = {}, const speckle::utility::String& trace = {}) :
				base{}, message{mess}, error{err}, stackTrace{trace} {}
		
		// MARK: - Public variables (NB: Assuming to class invariants or overrides for this data, so making public for simplicity)
		
			///The exception message, i.e. what happened
		speckle::utility::String message;
			///The exception details (name? - use is not clear)
		speckle::utility::String error;
			///Stack trace to where the exception was thrown
		speckle::utility::String stackTrace; //TODO: Not currently implemented - can add when required
		
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

#endif	//CONNECTOR_INTERFACE_BRIDGE_ERROR_REPORT
