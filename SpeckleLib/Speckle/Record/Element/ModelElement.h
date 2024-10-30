#ifndef SPECKLE_RECORD_MODEL_ELEMENT
#define SPECKLE_RECORD_MODEL_ELEMENT

#include "Speckle/Primitive/Primitive3D.h"
#include "Speckle/Record/Classification/Classified.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Record/Property/Propertied.h"

namespace speckle::record::attribute {
	class Finish;
}

namespace speckle::record::element {
	
	/*!
	 Base class for model elements, i.e. with 3D bodies and typically representing the project model
	 */
	class ModelElement : public Element, public classify::Classified, public property::Propertied {
	public:

		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<ModelElement>;
			///Shared pointer
		using Shared = std::shared_ptr<ModelElement>;
			///Optional
		using Option = std::optional<ModelElement>;
			///A model element 3D body primitive
		using Body = active::container::Vector<speckle::primitive::Primitive3D>;
		
		// MARK: - Constructors
		
		/*!
		 Get a mesh finish from the cache
		 @param finishID A finish ID
		 @return A pointer to the requested finish (nullptr on failure)
		 */
		static record::attribute::Finish* getFinish(const utility::Guid& finishID);
		/*!
		 Add a mesh finish to the cache
		 @param finishID A finish ID
		 @param finish The mesh finish
		 @return A pointer to the cached finish
		 */
		static record::attribute::Finish* cacheFinish(const utility::Guid& finishID, const record::attribute::Finish& finish);
		/*!
		 Reset the Archicad material cache
		 */
		static void resetCache();
		
		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		ModelElement();
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 @param unit The record unit type
		 */
		ModelElement(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID,
					 std::optional<active::measure::LengthType> unit = active::measure::LengthType::metre);
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ModelElement(const ModelElement& source);
		/*!
		 Destructor
		 */
		~ModelElement();


		// MARK: - Functions (const)

		/*!
		 Get the element body
		 @return An array of meshes from the element body (nullptr if no body data is available)
		 */
		virtual Body* getBody() const;
		
		// MARK: - Functions (mutating)


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
		class Data;
			///The element data
		mutable std::unique_ptr<Data> m_data;
		class FinishCache;
			///Cached finishes (API lookup is very slow)
		static std::unique_ptr<FinishCache> m_finishCache;
	};

}

#endif	//SPECKLE_RECORD_MODEL_ELEMENT
