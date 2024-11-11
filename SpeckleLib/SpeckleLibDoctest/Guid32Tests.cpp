#include "ActiveLibDoctest/TestingPlatforms.h"

#include "Speckle/Utility/Guid64.h"

using namespace speckle::utility;

TEST_SUITE(TESTQ(GuidTests)) TEST_SUITE_OPEN

	  ///Tests for creating/modifying guids
  TEST_CASE(TESTQ(testGuidContent)) {
	  Guid64 badGuid{String{"abcd"}};
	  CHECK_MESSAGE(!badGuid, TEST_MESSAGE(Guid64 created from bad data));
  }

TEST_SUITE_CLOSE
