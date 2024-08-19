#include "Connector/Interface/Browser/Bridge/Account/GetAccounts.h"

#include "Active/Serialise/CargoHold.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::record::cred;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<ContainerWrap<Vector, Account>, Vector<Account>>;

}

/*--------------------------------------------------------------------
	Constructor
 
	bridge: The parent bridge object (provides access to bridge methods)
  --------------------------------------------------------------------*/
GetAccounts::GetAccounts() : JSBridgeMethod{"GetAccounts", [&]() { return getAccounts(); }} {}


/*--------------------------------------------------------------------
	Get an argument instance for the function (used to deserialise/unpack incoming arguments)
 
	return: An argument instance
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetAccounts::getArgument() const { return nullptr; }


/*--------------------------------------------------------------------
	Get the accounts
 
	return: The accounts (empty array when none defined)
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetAccounts::getAccounts(void) const {
	Vector<Account> accounts;
	///TODO: Get the accounts here - returning an empty array for testing only
	return std::make_unique<WrappedValue>(accounts);
} //GetAccounts::getAccounts
