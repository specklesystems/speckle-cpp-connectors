#ifndef SPECKLE_RECORD_ATTRIBUTE_LAYER
#define SPECKLE_RECORD_ATTRIBUTE_LAYER

#include "Speckle/Record/Attribute/Attribute.h"

#ifdef ARCHICAD
namespace ModelerAPI {
	class Layer;
}
#endif

namespace speckle::record::attribute {
	
	/*!
	 Class to represent the rendered layer on a 3D body, i.e. the surface colour/texture etc.
	 
	 In Archicad this attribute is represented by `API_LayerType`
	 */
	class Layer : public Attribute {
	public:

		// MARK: - Types
		
		using base = Attribute;
			///Unique pointer
		using Unique = std::unique_ptr<Layer>;
			///Shared pointer
		using Shared = std::shared_ptr<Layer>;
			///Optional
		using Option = std::optional<Layer>;
		
		// MARK: - Constants
		
#ifdef ARCHICAD
			///The layeres table identifier
		static constexpr active::utility::Guid table{active::utility::Guid::fromInt(API_LayerID)};
#endif

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Layer();
		/*!
		 Constructor
		 @param ID The attribute ID
		 */
		Layer(const database::BIMRecordID& ID);
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param attrData Archicad attribute data
		 @param tableID The ID of the parent table
		 */
		Layer(const API_Attribute& attrData, const database::BIMRecordID& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Layer(const Layer& source);
		/*!
		 Destructor
		 */
		~Layer();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		Layer* clonePtr() const override { return new Layer{*this}; }
		
		// MARK: - Operators
		
		/*!
		 Assignment operator
		 @param source The object to assign
		 @return A reference to this
		 */
		Layer& operator=(const Layer& source);

		// MARK: - Functions (const)

		/*!
		 Determine if the layer is hidden
		 @return True if the layer is hidden
		 */
		bool isHidden() const;
		/*!
		 Determine if the layer is locked
		 @return True if the layer is locked
		 */
		bool isLocked() const;
		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		speckle::utility::String getSpeckleType() const override { return "Objects.Other.Layer"; }
#ifdef ARCHICAD
		/*!
		 Get the (immutable) API attribute header data
		 @return The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		const API_Attr_Head& getHead() const override;
#endif
		
		// MARK: - Functions (mutating)

		/*!
		 Set whether the layer is hidden
		 @param state True if the layer is hidden
		 */
		void setHidden(bool state);
		/*!
		 Set whether the layer is locked
		 @param state True if the layer is locked
		 */
		void setLocked(bool state);
#ifdef ARCHICAD
		/*!
		 Get the (mutable) API attribute header data
		 @return The attribute header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		API_Attr_Head& getHead() override;
#endif

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
		/*!
		 Validate the cargo data
		 @return True if the data has been validated
		 */
		bool validate() override;

	protected:
		/*!
		 Get the attribute data to be written to the database
		 @return The attribute data (for internal use to write to the database)
		 */
		API_Attribute getDataOut() const override;

	private:
		/*!
		 Confirm the internal data, either loading from the BIM application or setting a default
		 */
		void confirmData() const;
		
		class Data;
			///The attribute data - mutable to support lazy loading
		mutable std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ATTRIBUTE_LAYER
