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
		
			///Unique pointer
		using Unique = std::unique_ptr<Propertied>;
			///Shared pointer
		using Shared = std::shared_ptr<Propertied>;
			///Optional
		using Option = std::optional<Propertied>;

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
