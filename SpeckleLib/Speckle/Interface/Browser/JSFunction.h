#ifndef SPECKLE_INTERFACE_JS_FUNCTION
#define SPECKLE_INTERFACE_JS_FUNCTION

#include "Active/Serialise/Inventory/Identity.h"
#include "Active/Serialise/Item/Wrapper/ItemWrap.h"
#include "Active/Serialise/Package/NullPackage.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Speckle/Interface/Browser/JSBinding.h"
#include "Speckle/Interface/Browser/NamedFunction.h"
#include "Speckle/Utility/String.h"

#include <functional>
#include <type_traits>
#include <utility>

namespace speckle::interfac::browser {
	
	/*!
	 Interface for a C++ function binding to a JavaScript function
	 @tparam Argument The function parameter type
	 @tparam Return The function return type
	 @tparam Binding The platform JS binding
	*/
	template<typename Argument, typename Return, typename Binding>
	class JSFunction : public NamedFunction<Argument, Return>, public Binding {
	public:

		// MARK: - Types
		
		using base = NamedFunction<Argument, Return>;
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
		 Get the bound function name
		 @return The bound function name
		 */
		speckle::utility::String getName() const override { return base::getName(); }
		/*!
		 Execute the function
		 @param param The input parameter
		 @return The function result
		 */
		typename Binding::ValueType execute(const typename Binding::ValueType& param) const override;
	};
	
	
	/*--------------------------------------------------------------------
		Execute the function
	 
		param: The input parameter
	 
		return: The function result
	  --------------------------------------------------------------------*/
	template<typename Argument, typename Return, typename Binding>
	Binding::ValueType JSFunction<Argument, Return, Binding>::execute(const typename Binding::ValueType& bindingParameters) const {
		typename Binding::Transport transport;
			//Process any returned result into the binding value type
		auto processResult = [&, transport](Return* outgoing) -> typename Binding::ValueType {
			if constexpr(!std::is_same<Return, void>::value) {
				typename Binding::ValueType result;
				transport.send(std::move(*outgoing), active::serialise::Identity{}, result);
				return result;
			}
			return typename Binding::ValueType{};
		};
			//Ensure void parameters and/or return values are handled correctly
		if constexpr(std::is_same<Argument, void>::value) {
			active::serialise::NullPackage null;
			if constexpr(std::is_same<Return, void>::value)
				base::execute(null);	//No parameters and no return value
			else {
				auto result = base::execute(null);	//No parameters with return value
				return processResult(dynamic_cast<Return*>(result.get()));
			}
		} else {
			Argument argument;
			if constexpr (std::is_base_of<active::serialise::Package, Argument>::value)
				transport.receive(active::serialise::PackageWrap{argument}, active::serialise::Identity{}, bindingParameters);
			else
				transport.receive(active::serialise::ItemWrap{argument}, active::serialise::Identity{}, bindingParameters);
			if constexpr(std::is_same<Return, void>::value)
				base::execute(argument);	//Parameters and no return value
 			else {
				auto result = base::execute(argument);	//Parameters with return value
				return processResult(dynamic_cast<Return*>(result.get()));
			}
		}
		return typename Binding::ValueType{};
	} //JSFunction<Argument, Return, Binding>::execute

}

#endif	//SPECKLE_INTERFACE_JS_FUNCTION
