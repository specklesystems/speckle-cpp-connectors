#ifndef CONNECTOR_INTERFACE_BRIDGE_GETSELECTION
#define CONNECTOR_INTERFACE_BRIDGE_GETSELECTION

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class GetSelection : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetSelection();

		// MARK: - Functions (const)
		
		/*!
		 Send a specified model
		 @param modelCardID The ID of the model to send
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GETSELECTION
