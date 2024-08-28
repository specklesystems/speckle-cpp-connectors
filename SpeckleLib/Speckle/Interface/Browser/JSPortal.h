#ifndef SPECKLE_INTERFACE_JS_PORTAL
#define SPECKLE_INTERFACE_JS_PORTAL

#include "Speckle/Interface/Browser/JSObject.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include <JavascriptEngine.hpp>
#endif

namespace speckle::interfac::browser {
	
	/*!
	 Interface for a portal to a JavaScript environment, e.g. CEF
	 @tparam FunctionBinding The platform JS binding
	 
	 Platform-specific components should implement this object to interact with the Speckle JS library
	*/
	template<typename FunctionBinding = PlatformBinding>
	class JSPortal : protected std::vector<std::shared_ptr<JSObject<FunctionBinding>>> {
	public:
		
		// MARK: - Types
		
		using base = std::vector<std::shared_ptr<JSObject<FunctionBinding>>>;
		
		// MARK: - Constructors
		
		/*!
		 Destructor
		 */
		virtual ~JSPortal() {}

		// MARK: - Functions (mutating)
		
		/*!
		 Execute JS code
		 @param code The JS code
		 @return True if the code was successfully executed
		 */
		bool execute(const speckle::utility::String& code) const;
		/*!
		 Install a JS function object
		 @param object The object to install
		 @return True if the object was successfully installed
		 */
		bool install(std::shared_ptr<JSObject<FunctionBinding>> object);
		
	protected:
#ifdef ARCHICAD
		/*!
		 Get the portal Javascript engine
		 @return The portal Javascript engine
		 */
		virtual std::shared_ptr<JavascriptEngine> getJSEngine() const = 0;
#endif
	};
	
	
	/*--------------------------------------------------------------------
		Execute JS code
	 
		code: The JS code
	 
		return: True if the code was successfully executed
	  --------------------------------------------------------------------*/
	template<typename FunctionBinding>
	bool JSPortal<FunctionBinding>::execute(const speckle::utility::String& code) const {
#ifdef ARCHICAD
		try {
			auto engine = getJSEngine();

			OutputDebugString((LPCTSTR)speckle::utility::String{ "\nExecuted:\n" + code}.operator std::u16string().data());

			auto result = engine ? engine->ExecuteJS(code) : false;
			return result;
		} catch(...) {
			///TODO: Need to discuss the best course of action to notify of a failure
		}
#endif
		return false;
	} //JSPortal<FunctionBinding>::execute
	
	
	/*--------------------------------------------------------------------
		Install a JS function object
	 
		object: The object to install
	 
		return: True if the object was successfully installed
	  --------------------------------------------------------------------*/
	template<typename FunctionBinding>
	bool JSPortal<FunctionBinding>::install(std::shared_ptr<JSObject<FunctionBinding>> object) {
#ifdef ARCHICAD
		try {
			auto engine = getJSEngine();
			if (!engine)
				return false;
				//Define the JS object
			JS::Object* acObject = new JS::Object(object->getName());
				//Add all the functions supported by this object
			for (auto& function : *object) {
				acObject->AddItem(new JS::Function(function->getName(), [&](GS::Ref<JS::Base> args) {
					try {
							//NB: All JS functions enter at this point
						return function->execute(args);
					} catch(...) {
						///TODO: Need to discuss the best course of action to notify of a failure
					}
					return GS::Ref<JS::Base>{};
				}));
			}
				//And finally register the object
			if (engine->RegisterAsynchJSObject(acObject)) {
				base::push_back(object);
				object->setPortal(*this);
				return true;
			}
		} catch(...) {
			///TODO: Need to discuss the best course of action to notify of a failure
		}
#endif
		return false;
	} //JSPortal<FunctionBinding>::install
	
}

#endif	//SPECKLE_INTERFACE_JS_PORTAL
