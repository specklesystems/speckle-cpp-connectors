#ifndef SPECKLE_INTERFACE_FUNCTIONAL
#define SPECKLE_INTERFACE_FUNCTIONAL

#include "Active/Serialise/Cargo.h"
#include "Speckle/Utility/String.h"

namespace speckle::interfac::browser {
	
	/*!
	 Interface for objects providing an executable function
	 @tparam Packaging The packaging type for transporting input parameters and results (Cargo is used as the default for serialisation)
	*/
	template<typename Packaging = active::serialise::Cargo>
	class Functional : virtual public active::utility::Cloner {
	public:
		/*!
		 Destructor
		 */
		virtual ~Functional() {};

		// MARK: - Functions (const)
		
		/*!
		 Get the function name
		 @return The function name
		 */
		virtual speckle::utility::String getName() const = 0;
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		virtual std::unique_ptr<Packaging> getArgument() const = 0;
		/*!
		 Execute the function
		 @param param The transported input parameter
		 @return The data to transport back to the caller (nullptr if the result is void)
		 */
		virtual std::unique_ptr<Packaging> execute(const Packaging& param) const = 0;
	};

}

#endif	//SPECKLE_INTERFACE_FUNCTIONAL
