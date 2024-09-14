/*!
Copyright 2024 Ralph Wessel and Hugh Wessel
Distributed under the MIT License (See accompanying file LICENSE.txt or copy at https://opensource.org/license/mit/)
*/

#include "Connector/Record/Model/Filter/FilterMover.h"

#include "Connector/Record/Model/Filter/DirectSelectionSendFilter.h"

using namespace active::serialise;
using namespace active::utility;
using namespace connector::record;

namespace {
	
		///The tag used to identify a Speckle type name value
	const char* attributeTag = "typeDiscriminator";
		///Identity for a SenderModelCard
	const char* DirectSelectionTypeName = "DirectSelectionSendFilter";
	
}

	///The handler for model card packages
std::shared_ptr<active::serialise::Handler> FilterMover::m_handler = std::make_shared<active::serialise::Handler>(attributeTag);


/*--------------------------------------------------------------------
	Constructor (for deserialisation)
 
	handler: A package handler to reconstruct incoming packages
  --------------------------------------------------------------------*/
FilterMover::FilterMover() : Mover{m_handler} {
	validateHandler();
} //FilterMover::FilterMover


/*--------------------------------------------------------------------
	Constructor (for serialisation)
 
	outgoing: An outgoing package
  --------------------------------------------------------------------*/
FilterMover::FilterMover(const active::serialise::Package& outgoing) : Mover{outgoing, m_handler} {
	validateHandler();
} //FilterMover::FilterMover


/*--------------------------------------------------------------------
	Constructor (when it will be used for serialisation only on an existing object)
 
	package: A reference to the member variable
  --------------------------------------------------------------------*/
FilterMover::FilterMover(active::serialise::PackageUniqueWrap&& package) : Mover{std::move(package), m_handler} {
	
} //FilterMover::FilterMover


/*--------------------------------------------------------------------
	Ensure the handler is populated
  --------------------------------------------------------------------*/
void FilterMover::validateHandler() {
	if (!m_handler->empty())
		return;
	m_handler->add<DirectSelectionSendFilter>(DirectSelectionTypeName);
} //FilterMover::validateHandler
