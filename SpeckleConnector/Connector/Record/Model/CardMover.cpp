/*!
Copyright 2024 Ralph Wessel and Hugh Wessel
Distributed under the MIT License (See accompanying file LICENSE.txt or copy at https://opensource.org/license/mit/)
*/

#include "Connector/Record/Model/CardMover.h"

#include "Connector/Record/Model/SenderModelCard.h"

using namespace active::serialise;
using namespace active::utility;
using namespace connector::record;

namespace {
	
		///The tag used to identify a Speckle type name value
	const char* attributeTag = "typeDiscriminator";
		///Identity for a SenderModelCard
	const char* senderCardTypeName = "SenderModelCard";
		///Identity for a ReceiverModelCard
	const char* receiverCardTypeName = "ReceiverModelCard";
	
}

	///The handler for model card packages
std::shared_ptr<active::serialise::Handler> CardMover::m_handler = std::make_shared<active::serialise::Handler>(attributeTag);


/*--------------------------------------------------------------------
	Constructor (for deserialisation)
 
	handler: A package handler to reconstruct incoming packages
  --------------------------------------------------------------------*/
CardMover::CardMover() : Mover{m_handler} {
	validateHandler();
} //CardMover::CardMover


/*--------------------------------------------------------------------
	Constructor (for serialisation)
 
	outgoing: An outgoing package
  --------------------------------------------------------------------*/
CardMover::CardMover(const active::serialise::Package& outgoing) : Mover{outgoing, m_handler} {
	validateHandler();
} //CardMover::CardMover


/*--------------------------------------------------------------------
	Ensure the handler is populated
  --------------------------------------------------------------------*/
void CardMover::validateHandler() {
	if (!m_handler->empty())
		return;
	m_handler->add<SenderModelCard>(senderCardTypeName);
} //CardMover::validateHandler
