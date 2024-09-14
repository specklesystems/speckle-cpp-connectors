#ifndef CONNECTOR_MODEL_CARD_MOVER
#define CONNECTOR_MODEL_CARD_MOVER

#include "Active/Serialise/Package/Wrapper/Mover.h"

namespace connector::record {
	
	/*!
	 Wrapper to box/unbox objects during (de)serialisation, reading/writing a specified attribute to determine object type
	 
	 Note that a derived class could also define the package handler, allowing the wrapper to be created via a default constructor that is
	 automatically bound to a set of internally defined object types
	 */
	class CardMover : public active::serialise::Mover {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		CardMover();
		/*!
		 Constructor (for serialisation)
		 @param outgoing An outgoing package
		 */
		CardMover(const active::serialise::Package& outgoing);
		
	private:
		/*!
		 Ensure the handler is populated
		 */
		static void validateHandler();
		
			///The handler for model card packages
		static std::shared_ptr<active::serialise::Handler> m_handler;
	};

}
	
#endif //CONNECTOR_MODEL_CARD_MOVER
