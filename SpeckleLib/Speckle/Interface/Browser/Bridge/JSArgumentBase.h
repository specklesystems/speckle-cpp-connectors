#ifndef SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_BASE
#define SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_BASE

#include "Active/Serialise/Package/Package.h"
#include "Speckle/Utility/String.h"

namespace speckle::interfac::browser::bridge {
		
	/*!
	 Interface for objects used as arguments for JS bridge arguments
	 
	 Enables deserialisation to recognise individual parameters in the serialised JS method aergument, e.g. "0", "1", etc
	*/
	class JSArgumentBase {
	public:
		/*!
		 Destructor
		 */
		virtual ~JSArgumentBase() {}
		
		// MARK: - Functions (const)
		
		/*!
		 Determine if the object is used as an argument template
		 @return True if the object is used as an argument template
		 */
		bool isArgumentTemplate() const { return m_isArgumentTemplate; }
		
		// MARK: - Functions (mutating)
		
		/*!
		 Set whether the object is used as an argument template
		 @param state True if the object is used as an argument template
		 */
		void setArgumentTemplate(bool state)  { m_isArgumentTemplate = state; }
		
	private:
			///True if the object is used as an argument template
		bool m_isArgumentTemplate = false;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_ARGUMENT_BASE
