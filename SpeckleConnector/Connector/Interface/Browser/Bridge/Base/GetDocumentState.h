#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_DOCUMENT_STATE
#define CONNECTOR_INTERFACE_BRIDGE_GET_DOCUMENT_STATE

#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the information about the active document
	*/
	class GetDocumentState : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetDocumentState();

		// MARK: - Functions (const)
		
		/*!
		 Get the document info
		 @return The document info
		 */
		std::unique_ptr<active::serialise::Cargo> run(void) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_DOCUMENT_STATE
