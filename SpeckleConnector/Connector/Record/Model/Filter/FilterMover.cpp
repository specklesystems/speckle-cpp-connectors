/*!
Copyright 2024 Ralph Wessel and Hugh Wessel
Distributed under the MIT License (See accompanying file LICENSE.txt or copy at https://opensource.org/license/mit/)
*/

#include "Connector/Record/Model/Filter/FilterMover.h"

#include "Connector/Record/Model/Filter/ArchicadEverythingFilter.h"
#include "Connector/Record/Model/Filter/ArchicadSelectionFilter.h"

using namespace active::serialise;
using namespace active::utility;
using namespace connector::record;

namespace {
	
		///The tag used to identify a Speckle type name value
	const char* attributeTag = "typeDiscriminator";
	///Identity for selecting everything
	const char* ArchicadEverythingTypeName = "ArchicadEverythingFilter";
	///Identity for a selection filter
	const char* ArchicadSelectionTypeName = "ArchicadSelectionFilter";

	/*--------------------------------------------------------------------
		Ensure the handler is populated

		handler: The filter handler to validate

		return: A reference to the handler
	  --------------------------------------------------------------------*/
	std::shared_ptr<active::serialise::Handler>& validateHandler(std::shared_ptr<active::serialise::Handler>& handler) {
		if (!handler->empty())
			return handler;
		handler->add<ArchicadEverythingFilter>(ArchicadEverythingTypeName);
		handler->add<ArchicadSelectionFilter>(ArchicadSelectionTypeName);
		return handler;
	} //validateHandler

}

	///The handler for model card packages
std::shared_ptr<active::serialise::Handler> FilterMover::m_handler = std::make_shared<active::serialise::Handler>(attributeTag);


/*--------------------------------------------------------------------
	Constructor (for deserialisation)
 
	handler: A package handler to reconstruct incoming packages
  --------------------------------------------------------------------*/
FilterMover::FilterMover() : Mover{validateHandler(m_handler)} {
} //FilterMover::FilterMover


/*--------------------------------------------------------------------
	Constructor (for serialisation)
 
	outgoing: An outgoing package
  --------------------------------------------------------------------*/
FilterMover::FilterMover(const active::serialise::Package& outgoing) : Mover{outgoing, validateHandler(m_handler)} {
} //FilterMover::FilterMover


/*--------------------------------------------------------------------
	Constructor (when it will be used for serialisation only on an existing object)
 
	package: A reference to the member variable
  --------------------------------------------------------------------*/
FilterMover::FilterMover(active::serialise::PackageUniqueWrap&& package) : Mover{std::move(package), validateHandler(m_handler)} {
} //FilterMover::FilterMover
