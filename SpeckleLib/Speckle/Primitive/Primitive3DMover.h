#ifndef CONNECTOR_MODEL_PRIMITIVE3D_MOVER
#define CONNECTOR_MODEL_PRIMITIVE3D_MOVER

#include "Active/Serialise/Package/Wrapper/Mover.h"

namespace speckle::primitive {
	
	/*!
	 Wrapper to box/unbox 3D primitives during (de)serialisation
	 
	 Primitives are polymorphic - this class ensures the type information is included when a primitive is serialised
	 and the correct object type is constructed on deserialisation
	 */
	class Primitive3DMover : public active::serialise::Mover {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		Primitive3DMover();
		/*!
		 Constructor (for serialisation)
		 @param outgoing An outgoing package
		 */
		Primitive3DMover(const active::serialise::Package& outgoing);
		
	private:
			///The handler for model card packages
		static std::shared_ptr<active::serialise::Handler> m_handler;
	};

}
	
#endif //CONNECTOR_MODEL_PRIMITIVE3D_MOVER
