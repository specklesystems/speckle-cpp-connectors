#ifndef SPECKLE_SERIALISE_DETACHED_WRAP
#define SPECKLE_SERIALISE_DETACHED_WRAP

#include "Active/Serialise/Package/Package.h"

#include "Speckle/Database/Identity/RecordID.h"

namespace active::utility {
	class BufferOut;
}

namespace speckle::serialise {
	
	/*!
	 Lightweight interface wrapper for a detached object
	 
	 The wrapper generates a new reference ID for the object during serialisation, embedding it in the output
	 */
	class DetachedWrap : public active::serialise::Package, public std::reference_wrapper<active::serialise::Package> {
	public:
		
		// MARK: - Types
		
			///Item reference base
		using base = std::reference_wrapper<active::serialise::Package>;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param source The source package to wrap
		 @param bufferOut The serialisation output buffer
		 */
		DetachedWrap(active::serialise::Package& source, active::utility::BufferOut& bufferOut) : base(source), m_bufferOut{&bufferOut} {}
		/*!
		 Constructor
		 @param source The source package to wrap
		 @param bufferOut The serialisation output buffer
		 */
		DetachedWrap(const active::serialise::Package& source, active::utility::BufferOut& bufferOut) :
				DetachedWrap{const_cast<active::serialise::Package&>(source), bufferOut} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		DetachedWrap(const DetachedWrap& source) : base(source), m_reference{source.m_reference} {}
			
		// MARK: - Functions (const)
		
		/*!
		 Get the detached object reference
		 @return The detached object reference (nullopt if the reference has not been determined yet - call after (de)serialisation)
		 */
		std::optional<database::RecordID> getReference() const { return m_reference; }
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
			///The detached record reference
		mutable std::optional<database::RecordID> m_reference;
			///The serialisation output buffer
		active::utility::BufferOut* m_bufferOut;
	};
	
}

#endif	//SPECKLE_SERIALISE_DETACHED_WRAP
