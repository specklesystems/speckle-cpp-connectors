#ifndef CONNECTOR_INTERFACE_BRIDGE_HIGHLIGHT_MODEL
#define CONNECTOR_INTERFACE_BRIDGE_HIGHLIGHT_MODEL

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Connector/Interface/Browser/Bridge/Config/Arg/ConnectorConfig.h"
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
	class HighlightModel : public speckle::interfac::browser::bridge::BridgeMethod<SendArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		HighlightModel();

		// MARK: - Functions (const)
		
		/*!
		 Highlight elements linked to the model card selection
		 @param modelCardID The ID of the target model card
		 */
		void run(const speckle::utility::String& modelCardID) const;
	};

}

#endif	//CONNECTOR_INTERFACE_HIGHLIGHT_MODEL
