#include "Connector/Record/Model/CardMover.h"

#include "Connector/Record/Model/ReceiverModelCard.h"
#include "Connector/Record/Model/SenderModelCard.h"

using namespace connector::record;

namespace {
	
		///The tag used to identify a Speckle type name value
	const char* attributeTag = "typeDiscriminator";
		///Identity for a ReceiverModelCard
	const char* receiverCardTypeName = "ReceiverModelCard";
		///Identity for a SenderModelCard
	const char* senderCardTypeName = "SenderModelCard";
	
	/*--------------------------------------------------------------------
		Ensure the handler is populated
	 
	 	handler: The card handler to validate
	 
	 	return: A reference to the handler
	  --------------------------------------------------------------------*/
	std::shared_ptr<active::serialise::Handler>& validateHandler(std::shared_ptr<active::serialise::Handler>& handler) {
		if (!handler->empty())
			return handler;
		handler->add<ReceiverModelCard>(receiverCardTypeName);
		handler->add<SenderModelCard>(senderCardTypeName);
		return handler;
	} //CardMover::validateHandler
	
}

	///The handler for model card packages
std::shared_ptr<active::serialise::Handler> CardMover::m_handler = std::make_shared<active::serialise::Handler>(attributeTag);


/*--------------------------------------------------------------------
	Constructor (for deserialisation)
 
	handler: A package handler to reconstruct incoming packages
  --------------------------------------------------------------------*/
CardMover::CardMover() : Mover{validateHandler(m_handler)} {
} //CardMover::CardMover


/*--------------------------------------------------------------------
	Constructor (for serialisation)
 
	outgoing: An outgoing package
  --------------------------------------------------------------------*/
CardMover::CardMover(const active::serialise::Package& outgoing) : Mover{outgoing, validateHandler(m_handler)} {
} //CardMover::CardMover
