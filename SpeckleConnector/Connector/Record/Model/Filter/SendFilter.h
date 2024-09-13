#ifndef CONNECTOR_RECORD_SEND_FILTER
#define CONNECTOR_RECORD_SEND_FILTER

#include "Active/Serialise/Package/Package.h"
#include "Active/Utility/Cloner.h"
#include "Speckle/Utility/String.h"

namespace connector::record {
	
	/*!
	 Base class for element filters applied when a model is sent to a Speckle server
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
				m_name{nm}, m_summary{sum}, m_isDefault{isDef} {}
		/*!
		 Destructor
		 */
		virtual ~SendFilter() {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		virtual SendFilter* clonePtr() const override { return new SendFilter(*this); };

		// MARK: - Functions (const)
		
		/*!
			Get the filter name
			@return The filter name
		*/
		const speckle::utility::String& getName() const { return m_name; };
		/*!
			Get a summary description of the filter function, e.g. "All selected elements"
			@return The filter summary description
		*/
		const speckle::utility::String& getSummary() const { return m_name; };
		/*!
			Determine if this is the default filter for model sends
			@return True if this is the default filter
		*/
		bool isDefault() const { return m_isDefault; };
		
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
		
	private:
			///The filter name
		speckle::utility::String m_name;
			///A summary
		speckle::utility::String m_summary;
			///True if this is the default filter
		bool m_isDefault = false;
	};

}

#endif	//CONNECTOR_RECORD_SEND_FILTER
