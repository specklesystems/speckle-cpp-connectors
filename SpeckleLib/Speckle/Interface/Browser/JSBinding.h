#ifndef SPECKLE_INTERFACE_JS_BINDING
#define SPECKLE_INTERFACE_JS_BINDING

#include "Active/Utility/Cloner.h"
#include "Speckle/Utility/String.h"

namespace speckle::interfac::browser {
	
	/*!
	 Interface for the data binding of a function between JS and C++ objects
	 @tparam JSType The platform JS value type
	 @tparam Trans The transport between objects and platform JS value types
	*/
	template<typename JSType, typename Trans>
	class JSBinding : virtual public active::utility::Cloner {
	public:
		
		// MARK: - Types
		
		using ValueType = JSType;
		using Transport = Trans;
		
		// MARK: - Constructors
		
		virtual ~JSBinding() {}
		/*!
			Object cloning
			@return A clone of this object
		*/
		virtual JSBinding* clonePtr() const = 0;
		
		// MARK: - Functions (const)
		
		/*!
		 Get the bound function name
		 @return The bound function name
		 */
		virtual speckle::utility::String getName() const = 0;
		/*!
		 Execute the function
		 @param param The input parameter
		 @return The function result
		 */
		virtual JSType execute(const JSType& param) const = 0;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BINDING
