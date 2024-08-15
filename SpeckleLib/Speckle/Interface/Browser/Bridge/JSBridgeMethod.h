#ifndef SPECKLE_INTERFACE_JS_BRIDGE_METHOD
#define SPECKLE_INTERFACE_JS_BRIDGE_METHOD

#include "Speckle/Interface/Browser/JSObject.h"
#include "Speckle/Interface/Browser/NamedFunction.h"

#ifdef ARCHICAD
#include "Speckle/Serialise/JSBase/JSBaseTransport.h"

#include <Ref.hpp>
#include <JSValues.hpp>
#else
#include "Active/Serialise/JSON/JSONTransport.h"
#endif

namespace speckle::interface::browser::bridge {
	
	/*!
	 A class to provide JS bridging for Speckle connectors using a table of defined methods
	*/
	template<typename Param, typename Return>
	class JSBridgeMethod : public NamedFunction<Param, Return> {
		
		// MARK: - Types
		
		using base = NamedFunction<Param, Return>;
			//A bridged C++ function
		using Function = base::Function;
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param name The function name
		 @param function The C++ function bound to the name
		 */
		JSBridgeMethod(const speckle::utility::String& name, Function function) : base{name, function} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		JSBridgeMethod(const JSBridgeMethod& source) = default;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_METHOD
