#ifndef CONNECTOR_INTERFACE_BRIDGE_OPEN_URL
#define CONNECTOR_INTERFACE_BRIDGE_OPEN_URL

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class ConnectorConfig;
	
		///Argument parameter for a string
	using StringHold = active::serialise::CargoHold<active::serialise::ValueWrap<speckle::utility::String>, speckle::utility::String>;
		///Argument type for this method
	using SendArgs = speckle::interfac::browser::bridge::JSArgType<StringHold>;

	/*!
	 JS Function class to highlight elements from the selected model card in the open document
	*/
	class OpenUrl : public speckle::interfac::browser::bridge::BridgeMethod<SendArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		OpenUrl();

		// MARK: - Functions (const)
		
		/*!
		 Opens an url
		 @param url The URL to open
		 */
		void run(const speckle::utility::String& url) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_OPEN_URL
