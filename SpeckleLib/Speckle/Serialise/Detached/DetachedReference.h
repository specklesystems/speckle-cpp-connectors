#ifndef SPECKLE_SERIALISE_DETACHED_REFERENCE
#define SPECKLE_SERIALISE_DETACHED_REFERENCE

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Serialise/Detached/DetachedWrap.h"
#include "Speckle/Utility/String.h"

namespace speckle::serialise {
	
	/*!
	 Wrapper for references to a detached (serialised) object
	 
	 An object holding a child object that should be serialised as a detached object should wrap the child in this package for (de)serialisation
	 */
	class DetachedReference : public active::serialise::Package, std::reference_wrapper<active::serialise::Package> {
	public:
		
		// MARK: - Types
		
			///Item reference base
		using base = std::reference_wrapper<active::serialise::Package>;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param source The source package to wrap
		 */
		DetachedReference(Package& source) : base{source} {}
		/*!
		 Constructor
		 @param source The source package to wrap
		 */
		DetachedReference(const active::serialise::Package& source) : base(const_cast<active::serialise::Package&>(source)) {}
		DetachedReference(const DetachedReference& source) = delete;
		/*!
		 Destructor
		 */
		~DetachedReference() override = default;
			
		// MARK: - Functions (const)
		
		/*!
		 Fill an inventory with the cargo items
		 @param inventory The inventory to receive the cargo items
		 @return True if items have been added to the inventory
		 */
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure)
		 */
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
	
		// MARK: - Functions (mutating)
		
		/*!
		 Set to the default package content
		*/
		void setDefault() override;
		/*!
		 Validate the cargo data
		 @return True if the data has been validated
		 */
		bool validate() override;
		
	private:
			///The reference value, i.e. a hash of the object content
		mutable std::optional<database::RecordID> m_reference;
			///The reference type
		utility::String m_type;
	};
	
}

#endif	//SPECKLE_SERIALISE_DETACHED_REFERENCE
