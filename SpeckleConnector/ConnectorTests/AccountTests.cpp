#include "ActiveLibDoctest/TestingPlatforms.h"

#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Active/Utility/BufferIn.h"
#include "Speckle/Record/Credentials/Account.h"

using namespace active::serialise;
using namespace active::serialise::json;
using namespace active::serialise::json;
using namespace speckle::record::cred;
using namespace speckle::utility;

namespace {
	
	const char* accountJSON =  "{\"id\":\"194B6E32061EB588B31D8CBC167DAAD8\",\"token\":\"547e30bcd0f0e377fe422019f388295e37c583a99f\",\"refreshToken\":\"7d23031b44856e768b5951161c9bc8378f663eefce\",\"isDefault\":false,\"isOnline\":false,\"serverInfo\":{\"name\":\"My new Speckle Server\",\"company\":\"Unknown Company\",\"version\":\"2.20.2-branch.testing.138676-e8c8291\",\"adminContact\":\"n/a\",\"description\":\"This a community deployment of a Speckle Server.\",\"frontend2\":true,\"url\":\"https://testing.speckle.dev\",\"migration\":null},\"userInfo\":{\"id\":\"e66694013f\",\"name\":\"oguzhan@speckle.systems\",\"email\":\"oguzhan@speckle.systems\",\"company\":null,\"avatar\":null,\"streams\":{\"totalCount\":2},\"commits\":{\"totalCount\":5}}}";
	
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
