#include "ActiveLibDoctest/TestingPlatforms.h"

#include "Connector/Record/Model/CardMover.h"
#include "Connector/Record/Model/ModelCard.h"
#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Utility/BufferIn.h"
#include "Speckle/Utility/String.h"

namespace {
	
	speckle::utility::String json{"{\n\
		 \"typeDiscriminator\": \"SenderModelCard\",\n\
		  \"modelCardId\": \"8a007ac08b43771ec20d\",\n\
		  \"modelId\": \"c9b5a4fa94\",\n\
		  \"projectId\": \"ce9a0d130e\",\n\
		  \"workspaceId\": \"10ee1c0f73\",\n\
		  \"accountId\": \"5724C96F3FF796628292B6E67E86CC2F\",\n\
		  \"serverUrl\": \"https://app.speckle.systems\",\n\
		  \"expired\": false,\n\
		  \"sendFilter\": {\n\
		  \"typeDiscriminator\": \"ArchicadSelectionFilter\",\n\
		  \"selectedObjectIds\": [\n\
		  \"7B531D03-0219-420F-BE86-633451AEF19B\"\n\
				 ],\n\
		\"name\": \"Selection\",\n\
		\"summary\": \"1 objects selected.\"\n\
			 }\n\
		 }"};

}
using namespace speckle::utility;

TEST_SUITE(TESTQ(ModelCardSerialiseTest)) TEST_SUITE_OPEN

	  ///Test for deserialising a ModelCard
  TEST_CASE(TESTQ(deserialiseModelCard)) {
	  using CardHold = active::serialise::CargoHold<connector::record::CardMover, connector::record::ModelCard>;
	  CardHold result;
	  active::serialise::json::JSONTransport().receive(std::forward<CardHold&&>(result), active::serialise::Identity{}, json);
  }

TEST_SUITE_CLOSE
