#ifndef SPECKLE_INTERFACE_JS_OBJECT
#define SPECKLE_INTERFACE_JS_OBJECT

#include "Active/Container/Vector.h"
#include "Speckle/Interface/Browser/JSFunction.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"

namespace speckle::interfac::browser {
	
	template<typename FunctionBinding>
	class JSPortal;
	
	/*!
	 Interface for a JavaScript object binding to C++ functions
	 @tparam FunctionBinding The platform JS binding (defines argument/result serialisation and transport mechanism to (de)serialise)
	*/
	template<typename FunctionBinding = PlatformBinding>
	class JSObject : public std::vector<std::shared_ptr<FunctionBinding>> {
	public:

		// MARK: - Types
		
		using base = std::vector<std::shared_ptr<FunctionBinding>>;
		using size_type = base::size_type;

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param name The JS object name
		 @param toReserve The number of items to reserve space for in the vector
		 */
		JSObject(const speckle::utility::String& name, size_type toReserve = 0) : base{toReserve}, m_name{name} {}
		/*!
		 Constructor
		 @param name The JS object name
		 @param items The items to insert in the array
		*/
		template<class Derived>
		explicit JSObject(const speckle::utility::String& name, const std::initializer_list<Derived>& items) : base{items}, m_name{name} {}
		virtual ~JSObject() {}
		// MARK: - Functions (const)
		
		/*!
		 Get the object name (as expressed to JS)
		 @return The object name
		 */
		const auto& getName() const { return m_name; }

		// MARK: - Functions (mutating)
		
		/*!
		 Set the parent JS portal for this object
		 @param portal The JS portal
		 */
		void setPortal(const JSPortal<FunctionBinding>& portal) { m_portal = &portal; }
		
	protected:
			///The parent JS portal for this object
		const JSPortal<FunctionBinding>* m_portal = nullptr;
		
	private:
			///The JS object name
		speckle::utility::String m_name;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_OBJECT
