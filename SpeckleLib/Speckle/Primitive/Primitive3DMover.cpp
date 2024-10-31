#include "Speckle/Primitive/Primitive3DMover.h"
#include "Speckle/Primitive/Primitive3D.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Primitive/Polyline/Polyline.h"

using namespace active::serialise;
using namespace speckle::primitive;

namespace {
	
		///The tag used to identify a Speckle type name value
	const char* attributeTag = "speckle_type";
		///Identity for a Mesh
	const char* meshTypeName = "Objects.Geometry.Mesh";
		///Identity for a Polyline
	const char* polylineTypeName = "Objects.Geometry.Polyline";
	
	/*--------------------------------------------------------------------
		Ensure the handler is populated
	 
	 	handler: The card handler to validate
	 
	 	return: A reference to the handler
	  --------------------------------------------------------------------*/
	std::shared_ptr<active::serialise::Handler>& validateHandler(std::shared_ptr<active::serialise::Handler>& handler) {
		if (!handler->empty())
			return handler;
		handler->add<speckle::primitive::Polyline>(polylineTypeName);
		handler->add<Mesh>(meshTypeName);
		return handler;
	}
	
}

	///The handler for 3D primitive packages
std::shared_ptr<Handler> Primitive3DMover::m_handler = std::make_shared<Handler>(attributeTag);


/*--------------------------------------------------------------------
	Constructor (for deserialisation)
 
	handler: A package handler to reconstruct incoming packages
  --------------------------------------------------------------------*/
Primitive3DMover::Primitive3DMover() : Mover{validateHandler(m_handler)} {
} //CardMover::CardMover


/*--------------------------------------------------------------------
	Constructor (for serialisation)
 
	outgoing: An outgoing package
  --------------------------------------------------------------------*/
Primitive3DMover::Primitive3DMover(const active::serialise::Package& outgoing) : Mover{outgoing, validateHandler(m_handler)} {
} //CardMover::CardMover
