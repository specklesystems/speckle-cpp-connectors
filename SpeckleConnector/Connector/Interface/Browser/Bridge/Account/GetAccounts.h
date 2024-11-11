#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_ACCOUNTS
#define CONNECTOR_INTERFACE_BRIDGE_GET_ACCOUNTS

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the user's account(s)
	*/
	class GetAccounts : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetAccounts();

		// MARK: - Functions (const)
		
		/*!
		 Get the accounts
		 @return The accounts (empty array when none defined)
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_ACCOUNTS
