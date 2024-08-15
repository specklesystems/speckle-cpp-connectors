#ifndef SPECKLE_INTERFACE_JS_FUNCTION
#define SPECKLE_INTERFACE_JS_FUNCTION

#include "Active/Serialise/Inventory/Identity.h"
#include "Speckle/Interface/Browser/JSBinding.h"
#include "Speckle/Interface/Browser/NamedFunction.h"
#include "Speckle/Utility/String.h"

#include <functional>
#include <type_traits>

namespace speckle::interface::browser {
	
	/*!
	 Interface for a C++ function binding to a JavaScript function
	 @tparam Param The function parameter type
	 @tparam Return The function return type
	 @tparam Binding The platform JS binding
	*/
	template<typename Param, typename Return, typename Binding>
	class JSFunction : public NamedFunction<Param, Return>, public Binding {
	public:

		// MARK: - Types
		
		using base = NamedFunction<Param, Return>;
			//A bridged C++ function
		using Function = base::Function;

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param jsName The function name (as called from JavaScript)
		 @param function The C++ function bound to the name
		 */
		JSFunction(const speckle::utility::String& jsName, Function function) : base{jsName, function} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		JSFunction(const JSFunction& source) = default;

		// MARK: - Functions (const)
		
		/*!
		 Execute the function
		 @param param The input parameter
		 @return The function result
		 */
		Binding::ValueType execute(const Binding::ValueType param) const;
		
	private:
		speckle::utility::String m_name;
		Function m_function;
	};
	
	
	/*--------------------------------------------------------------------
		Execute the function
	 
		param: The input parameter
	 
		return: The function result
	  --------------------------------------------------------------------*/
	template<typename Param, typename Return, typename Binding>
	Binding::ValueType JSFunction<Param, Return, Binding>::execute(const typename Binding::ValueType bindingParameters) const {
		typename Binding::Transport transport;
			//Process any returned result into the binding value type
		auto processResult = [&, transport](const Return* outgoing) -> typename Binding::ValueType {
			if constexpr(!std::is_same<Return, void>::value) {
				typename Binding::ValueType result;
				transport.send(*outgoing, active::serialise::Identity{}, result);
				return result;
			}
			return typename Binding::ValueType{};
		};
			//Ensure void parameters and/or return values are handled correctly
		if constexpr(std::is_same<Param, void>::value) {
			if constexpr(std::is_same<Return, void>::value)
				m_function();	//No parameters and no return value
			else {
				auto result = m_function();	//No parameters with return value
				return processResult(&result);
			}
		} else {
			Param parameters;
			transport.receive(parameters, active::serialise::Identity{}, bindingParameters);
			if constexpr(std::is_same<Return, void>::value)
				m_function(parameters);	//Parameters and no return value
 			else {
				auto result = m_function(parameters);	//Parameters with return value
				return processResult(&result);
			}
		}
		return typename Binding::ValueType{};
	} //JSFunction<Param, Return, Binding>::execute

}

#endif	//SPECKLE_INTERFACE_JS_FUNCTION
