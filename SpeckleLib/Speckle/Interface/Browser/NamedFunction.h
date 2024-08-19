#ifndef SPECKLE_INTERFACE_NAMED_FUNCTION
#define SPECKLE_INTERFACE_NAMED_FUNCTION

#include "Speckle/Interface/Browser/JSBinding.h"

#include "Active/Serialise/Cargo.h"
#include "Active/Serialise/Inventory/Identity.h"
#include "Active/Utility/NameID.h"
#include "Speckle/Interface/Browser/Functional.h"
#include "Speckle/Utility/String.h"

#include <functional>
#include <type_traits>

namespace speckle::interface::browser {
	
		//The following allows a function type to be described that supports void parameters, i.e. std::function<Return(void) is not legal syntax
	namespace inner {
		template<typename Param, typename Return>
		struct FuncBase {};
		template<typename Param, typename Return>
		struct FuncTypedef {
			typedef std::function<std::unique_ptr<Return>(Param)> type;
		};
		template<typename Return>
		struct FuncTypedef<void, Return> {
			typedef std::function<std::unique_ptr<Return>()> type;
		};
		template<typename Param>
		struct FuncTypedef<Param, void> {
			typedef std::function<void(Param)> type;
		};
		template<>
		struct FuncTypedef<void, void> {
			typedef std::function<void()> type;
		};
	}
		///Function signature definition
	template<typename Param, typename Return>
	using FuncBase = typename inner::FuncTypedef<Param, Return>::type;
	
		
	/*!
	 Generic interface for anonymous functions bound to a specified name
	 @tparam Param The function parameter type
	 @tparam Return The function return type
	 @tparam Packaging The packaging type for transporting input parameters and results (Cargo is used as the default for serialisation)
	*/
	template<typename Param, typename Return, typename Packaging = active::serialise::Cargo>
	class NamedFunction : protected active::utility::NameID, virtual public Functional<Packaging> {
	public:

		// MARK: - Types
		
			//A bridged C++ function
		using Function = FuncBase<Param, Return>;

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param jsName The function name (as called from JavaScript)
		 @param function The C++ function bound to the name
		 */
		NamedFunction(const speckle::utility::String& jsName, Function function) : active::utility::NameID{jsName}, m_function{function} {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		NamedFunction(const NamedFunction& source) = default;
		/*!
		 Destructor
		 */
		virtual ~NamedFunction() {};

		// MARK: - Functions (const)
		
		/*!
		 Get the function name
		 @return The function name
		 */
		speckle::utility::String getName() const override { return name; }
		/*!
		 Execute the function
		 @param param The transported input parameter
		 @return The data to transport back to the caller (nullptr if the result is void)
		 */
		std::unique_ptr<Packaging> execute(const Packaging& param) const override;
		
	private:
		Function m_function;
	};
	
	
	/*--------------------------------------------------------------------
		Execute the function
	 
		param: The transported input parameter
	 
		return: The data to transport back to the caller (nullptr if the result is void)
	  --------------------------------------------------------------------*/
	template<typename Param, typename Return, typename Packaging>
	std::unique_ptr<Packaging> NamedFunction<Param, Return, Packaging>::execute(const Packaging& param) const {
		std::unique_ptr<Packaging> result;
			//Ensure void parameters and/or return values are handled correctly
		if constexpr(std::is_same<Param, void>::value) {
			if constexpr(std::is_same<Return, void>::value)
				m_function();	//No parameters and no return value
			else {
				auto outgoing = m_function();	//No parameters with return value
				result = std::move(outgoing);
			}
		} else {
			auto parameters = dynamic_cast<const Param*>(&param);
			if (parameters == nullptr)
				throw;	//NB: Throw a system exception here in future with a defined error
			if constexpr(std::is_same<Return, void>::value)
				m_function(*parameters);	//Parameters and no return value
			else {
				auto outgoing = m_function(*parameters);	//Parameters with return value
				result = std::move(outgoing);
			}
		}
		return result;
	} //NamedFunction<Param, Return, Packaging>::execute
	
}

#endif	//SPECKLE_INTERFACE_NAMED_FUNCTION
