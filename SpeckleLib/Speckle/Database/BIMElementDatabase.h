#ifndef CONNECTOR_DATABASE_BIM_DATABASE
#define CONNECTOR_DATABASE_BIM_DATABASE

#include "Speckle/Record/Element/Element.h"
#include "Speckle/Utility/Guid.h"

namespace active::event {
	class Subscriber;
}

namespace speckle::database {
	
	/*!
	 Database of model elements relating to a specific project
	 */
	class BIMElementDatabase {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		BIMElementDatabase();
		BIMElementDatabase(const BIMElementDatabase&) = delete;
		/*!
		 Destructor
		 */
		~BIMElementDatabase();
		
		// MARK: - Functions (const)
		
		/*!
		 Get a specified element
		 @param elementID The ID of the target element
		 @return The requested element (nullptr on failure)
		 */
		record::element::Element::Unique getElement(const utility::Guid& elementID) const;
		/*!
		 Get all model elements
		 @return All the elements
		 */
		active::container::Vector<record::element::Element> getElements() const;
		/*!
		 Write an element to storage
		 @param element The element to write
		 */
		void write(const record::element::Element& element) const;
		/*!
		 Erase an element
		 @param elementID The ID of the element to erase
		 */
		void erase(const speckle::utility::Guid& elementID) const;
		
	private:
		class Engine;
		class Store;
			///Model element database storage
		std::shared_ptr<Engine> m_engine;
		std::shared_ptr<Store> m_store;
	};

}

#endif	//CONNECTOR_DATABASE_BIM_DATABASE
