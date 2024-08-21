#ifndef SPECKLE_INTERFACE_JS_BRIDGE_METHOD
#define SPECKLE_INTERFACE_JS_BRIDGE_METHOD

#include "Speckle/Interface/Browser/JSObject.h"
#include "Speckle/Interface/Browser/NamedFunction.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeArgumentWrap.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethodBase.h"

#ifdef ARCHICAD
#include "Speckle/Serialise/JSBase/JSBaseTransport.h"

#include <Ref.hpp>
#include <JSValues.hpp>
#else
#include "Active/Serialise/JSON/JSONTransport.h"
#endif

namespace speckle::interfac::browser::bridge {
	
	/*!
	 A class to provide JS bridging for Speckle connectors using a table of defined methods
	 @tparam Argument The function parameter type
	 @tparam Return The function return type
	*/
	template<typename Argument, typename Return>
	class JSBridgeMethod : public NamedFunction<Argument, Return>, public virtual JSBridgeMethodBase {
	public:
		
		// MARK: - Types
		
		using base = NamedFunction<Argument, Return>;
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
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		JSBridgeMethod* clonePtr() const override = 0;
		
		// MARK: - Functions (const)
		
		/*!
		 Register the method argument type for a specified bridge
		 @param bridge The target bridge name
		 @return A reference to this
		 */
		JSBridgeMethod& registerArgument(const speckle::utility::String& bridge) override {
			if constexpr(!std::same_as<Argument, void>)
				JSBridgeArgumentWrap::defineArgument<Argument>(base::getName());
			return *this;
		}
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_METHOD
