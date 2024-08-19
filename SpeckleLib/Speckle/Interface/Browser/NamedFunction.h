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
		template<typename Argument, typename Return>
		struct FuncBase {};
		template<typename Argument, typename Return>
		struct FuncTypedef {
			typedef std::function<std::unique_ptr<Return>(Argument)> type;
		};
		template<typename Return>
		struct FuncTypedef<void, Return> {
			typedef std::function<std::unique_ptr<Return>()> type;
		};
		template<typename Argument>
		struct FuncTypedef<Argument, void> {
			typedef std::function<void(Argument)> type;
		};
		template<>
		struct FuncTypedef<void, void> {
			typedef std::function<void()> type;
		};
	}
		///Function signature definition
	template<typename Argument, typename Return>
	using FuncBase = typename inner::FuncTypedef<Argument, Return>::type;
	
		
	/*!
	 Generic interface for anonymous functions bound to a specified name
	 @tparam Argument The function parameter type
	 @tparam Return The function return type
	 @tparam Packaging The packaging type for transporting input parameters and results (Cargo is used as the default for serialisation)
	*/
	template<typename Argument, typename Return, typename Packaging = active::serialise::Cargo>
	class NamedFunction : protected active::utility::NameID, virtual public Functional<Packaging> {
	public:

		// MARK: - Types
		
			//A bridged C++ function
		using Function = FuncBase<Argument, Return>;

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
		 @param arg The transported function argument
		 @return The data to transport back to the caller (nullptr if the result is void)
		 */
		std::unique_ptr<Packaging> execute(const Packaging& arg) const override;
		
	private:
		Function m_function;
	};
	
	
	/*--------------------------------------------------------------------
		Execute the function
	 
		param: The transported input parameter
	 
		return: The data to transport back to the caller (nullptr if the result is void)
	  --------------------------------------------------------------------*/
	template<typename Argument, typename Return, typename Packaging>
	std::unique_ptr<Packaging> NamedFunction<Argument, Return, Packaging>::execute(const Packaging& package) const {
		std::unique_ptr<Packaging> result;
			//Ensure void parameters and/or return values are handled correctly
		if constexpr(std::is_same<Argument, void>::value) {
			if constexpr(std::is_same<Return, void>::value)
				m_function();	//No argument and no return value
			else {
				auto outgoing = m_function();	//No argument with return value
				result = std::move(outgoing);
			}
		} else {
			auto argument = dynamic_cast<const Argument*>(&package);
			if (argument == nullptr)
				throw;	//NB: Throw a system exception here in future with a defined error
			if constexpr(std::is_same<Return, void>::value)
				m_function(*argument);	//Argument and no return value
			else {
				auto outgoing = m_function(*argument);	//Argument with return value
				result = std::move(outgoing);
			}
		}
		return result;
	} //NamedFunction<Argument, Return, Packaging>::execute
	
}

#endif	//SPECKLE_INTERFACE_NAMED_FUNCTION
