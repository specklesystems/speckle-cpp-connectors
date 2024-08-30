#include "Connector/Interface/Browser/Bridge/Account/GetAccounts.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Record/Credentials/Account.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::record::cred;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<ContainerWrap<Vector, Account>, Vector<Account>>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetAccounts::GetAccounts() : BridgeMethod{"GetAccounts", [&]() {
		return run();
	}} {}


/*--------------------------------------------------------------------
	Get the accounts
 
	return: The accounts (empty array when none defined)
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetAccounts::run() const {
	Vector<Account> accounts;
	///TODO: Get the accounts here - returning an empty array for testing only
	return std::make_unique<WrappedValue>(accounts);
} //GetAccounts::run
