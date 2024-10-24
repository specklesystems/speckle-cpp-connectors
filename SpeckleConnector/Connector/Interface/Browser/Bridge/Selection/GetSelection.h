#ifndef CONNECTOR_INTERFACE_BRIDGE_GETSELECTION
#define CONNECTOR_INTERFACE_BRIDGE_GETSELECTION

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	class GetSelection : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		GetSelection();

		// MARK: - Functions (const)
		
		/*!
		 Get the current selection info 
		 based on the ArchiCAD mdoel selection
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GETSELECTION
