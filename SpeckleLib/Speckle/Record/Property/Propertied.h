#ifndef SPECKLE_RECORD_ELEMENT_PROPERTIED
#define SPECKLE_RECORD_ELEMENT_PROPERTIED

#include "Speckle/Record/Property/Property.h"

namespace speckle::record::property {
	
	/*!
	 Interface for records with attached properties
	 */
	class Propertied {
	public:

		// MARK: - Types
		
			///Unary predicate for filtering properties
		using Filter = std::function<bool(const Property&)>;
			///Unique pointer
		using Unique = std::unique_ptr<Propertied>;
			///Shared pointer
		using Shared = std::shared_ptr<Propertied>;
			///Optional
		using Option = std::optional<Propertied>;
			///Size type for indexing etc
		using size_type = typename std::size_t;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Propertied() {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Propertied(const Propertied& source) :
				m_properties{source.m_properties ? std::make_unique<std::vector<Property>>(*source.m_properties) : nullptr} {}
		/*!
		 Move constructor
		 @param source The object to move
		 */
		Propertied(Propertied&& source) noexcept : m_properties{std::move(source.m_properties)} {}
		/*!
		 Destructor
		 */
		virtual ~Propertied() {}

		// MARK: - Functions (const)

		/*!
		 Get the number of attached properties
		 @return The property count
		 */
		size_type getPropertyCount() const;
		/*!
		 Get the number of attached properties passing a specified filter
		 @param filter The property filter
		 @return The number of attached properties passing the filter
		 */
		size_type getPropertyCount(const Filter& filter) const;
		/*!
		 Get a specified attached property
		 @param index The index of the target property
		 @return The requested property
		 */
		const Property& getProperty(size_type index) const;
		/*!
		 Get a specified attached property using a filter
		 @param filter The property filter
		 @param index The index of the target property (counting only properties passing the filter)
		 @return The requested property
		 */
		const Property& getProperty(const Filter& filter, size_type index) const;
		/*!
		 Get the attached properties
		 @return The attached properties
		 */
		const std::vector<Property>& getProperties() const;
		
		// MARK: - Functions (mutating)

	private:
		/*!
		 Rebuild the list of properties
		 @return True if any properties were found
		 */
		bool rebuild() const;

			///The attached properties - mutable to support lazy loading
		mutable std::unique_ptr<std::vector<Property>> m_properties;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_PROPERTIED
