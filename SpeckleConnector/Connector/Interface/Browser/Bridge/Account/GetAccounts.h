#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_ACCOUNTS
#define CONNECTOR_INTERFACE_BRIDGE_GET_ACCOUNTS

#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"
#include "Speckle/Record/Credentials/Account.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetAccounts : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetAccounts();
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GetAccounts* clonePtr() const override { return new GetAccounts{*this}; }

		// MARK: - Functions (const)
		
		/*!
		 Get the accounts
		 @return The accounts (empty array when none defined)
		 */
		std::unique_ptr<active::serialise::Cargo> run(void) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_ACCOUNTS
