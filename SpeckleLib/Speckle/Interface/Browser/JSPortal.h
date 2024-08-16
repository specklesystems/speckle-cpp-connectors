#ifndef SPECKLE_INTERFACE_JS_PORTAL
#define SPECKLE_INTERFACE_JS_PORTAL

#include "Speckle/Interface/Browser/JSObject.h"
#include "Speckle/Interface/Browser/PlatformBinding.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include <JavascriptEngine.hpp>
#endif

namespace speckle::interface::browser {
	
	/*!
	 Interface for a portal to a JavaScript environment, e.g. CEF
	 @tparam FunctionBinding The platform JS binding
	 
	 Platform-specific components should implement this object to interact with the Speckle JS library
	*/
	template<typename FunctionBinding = PlatformBinding>
	class JSPortal : public std::vector<std::shared_ptr<JSObject<FunctionBinding>>> {
	public:
		
		// MARK: - Types
		
		using base = std::vector<std::shared_ptr<JSObject<FunctionBinding>>>;
		
		// MARK: - Constructors
		
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param engine The AC JS engine
		 */
		JSPortal(std::shared_ptr<JavascriptEngine> engine) : m_engine{engine} {}
#endif
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
		
	private:
#ifdef ARCHICAD
			//The portal JS engine - weak reference ensures the portal is not used beyond the lifetime of the UI context
		std::weak_ptr<JavascriptEngine> m_engine;
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
		std::shared_ptr<JavascriptEngine> engine{m_engine};
		return engine ? engine->ExecuteJS(code) : false;
#else
		return false;	//Implement as required
#endif
	} //JSPortal<FunctionBinding>::execute
	
	
	/*--------------------------------------------------------------------
		Install a JS function object
	 
		object: The object to install
	 
		return: True if the object was successfully installed
	  --------------------------------------------------------------------*/
	template<typename FunctionBinding>
	bool JSPortal<FunctionBinding>::install(std::shared_ptr<JSObject<FunctionBinding>> object) {
#ifdef ARCHICAD
		std::shared_ptr<JavascriptEngine> engine{m_engine};
		if (!engine)
			return false;
		JS::Object* acObject = new JS::Object(object->getName());
		for (auto& function : object) {
			acObject->AddItem(new JS::Function(function->getName(), [&] (GS::Ref<JS::Base> args) {
				return function->execute(args);
			}));
		}
		if (engine->RegisterAsynchJSObject(acObject)) {
			push_back(object);
			object->setPortal(*this);
			return true;
		}
#endif
		return false;
	} //JSPortal<FunctionBinding>::install
	
}

#endif	//SPECKLE_INTERFACE_JS_PORTAL
