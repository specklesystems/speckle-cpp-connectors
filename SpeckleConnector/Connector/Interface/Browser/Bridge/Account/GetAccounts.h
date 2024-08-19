#ifndef SPECKLE_INTERFACE_BRIDGE_GET_ACCOUNTS
#define SPECKLE_INTERFACE_BRIDGE_GET_ACCOUNTS

#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"
#include "Speckle/Record/Credentials/Account.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the names of the methods supported by the bridge
	*/
	class GetAccounts : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
			//active::serialise::ContainerWrap<active::container::Vector, speckle::record::cred::Account>> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetAccounts();
		/*!
		 Cppy constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetAccounts(const GetAccounts& source) = default;
		
		/*!
		 Object cloning
		 @return A clone of this object
		 */
		GetAccounts* clonePtr() const override { return new GetAccounts{*this}; }
		/*!
		 Get an argument instance for the function (used to deserialise/unpack incoming arguments)
		 @return An argument instance
		 */
		std::unique_ptr<active::serialise::Cargo> getArgument() const override;
				
	private:
		/*!
		 Get the accounts
		 @return The accounts (empty array when none defined)
		 */
		std::unique_ptr<active::serialise::Cargo> getAccounts(void) const;
	};

}

#endif	//SPECKLE_INTERFACE_BRIDGE_GET_ACCOUNTS
