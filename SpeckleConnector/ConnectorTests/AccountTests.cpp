#include "ActiveLibDoctest/TestingPlatforms.h"

#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Active/Utility/BufferIn.h"
#include "Speckle/Record/Credentials/Account.h"

using namespace active::serialise;
using namespace active::serialise::json;
using namespace active::serialise::json;
using namespace speckle::record::cred;
using namespace speckle::utility;

namespace {
	
	const char* accountJSON =  "{\"id\":\"d4468f8d6f1e4c48a899518795326ea8\",\"token\":\"42986a57b3304baa8773f937aa0b7bcd42986a57b3\",\"refreshToken\":\"00b01c421de4404889f30947de1506d3440c476432\",\"isDefault\":false,\"isOnline\":false,\"serverInfo\":{\"name\":\"My new Speckle Server\",\"company\":\"Unknown Company\",\"version\":\"2.20.2-branch.testing.138676-e8c8291\",\"adminContact\":\"n/a\",\"description\":\"This a community deployment of a Speckle Server.\",\"frontend2\":true,\"url\":\"https://testing.speckle.dev\",\"migration\":null},\"userInfo\":{\"id\":\"00b01c421d\",\"name\":\"someone@speckle.systems\",\"email\":\"someone@speckle.systems\",\"company\":null,\"avatar\":null,\"streams\":{\"totalCount\":2},\"commits\":{\"totalCount\":5}}}";
}


TEST_SUITE(TESTQ(AccountTests)) TEST_SUITE_OPEN
	
		//Test to receive account serialised as JSON from SQLite dbase
	TEST_CASE(TESTQ(testAccountReceive)) {
		Account account;
		try {
			JSONTransport().receive(PackageWrap{account}, Identity{}, String{accountJSON});
		} catch(...) {
			FAIL_CHECK(TEST_MESSAGE(Account deserialisation failed));
		}
	}

TEST_SUITE_CLOSE
