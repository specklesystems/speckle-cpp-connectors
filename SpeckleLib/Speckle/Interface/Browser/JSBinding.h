#ifndef SPECKLE_INTERFACE_JS_BINDING
#define SPECKLE_INTERFACE_JS_BINDING

#include "Active/Utility/Cloner.h"

namespace speckle::interface::browser {
	
	/*!
	 Interface for the data binding between JS and C++ objects
	 @tparam JSType The platform JS value type
	 @tparam Trans The transport between objects and platform JS value types
	*/
	template<typename JSType, typename Trans>
	class JSBinding : virtual public active::utility::Cloner {
	public:
		
		using ValueType = JSType;
		using Transport = Trans;

		virtual ~JSBinding() {}
		/*!
			Object cloning
			@return A clone of this object
		*/
		virtual JSBinding* clonePtr() const = 0;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BINDING
