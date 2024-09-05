#include "ActiveLibDoctest/TestingPlatforms.h"

#include "Speckle/Utility/Guid32.h"

using namespace speckle::utility;

TEST_SUITE(TESTQ(GuidTests)) TEST_SUITE_OPEN

	  ///Tests for creating/modifying guids
  TEST_CASE(TESTQ(testGuidContent)) {
	  Guid32 badGuid{String{"abcd"}};
	  CHECK_MESSAGE(!badGuid, TEST_MESSAGE(Guid32 created from bad data));
  }

TEST_SUITE_CLOSE
