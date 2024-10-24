#ifndef CONNECTOR_MODEL_CARD_MOVER
#define CONNECTOR_MODEL_CARD_MOVER

#include "Active/Serialise/Package/Wrapper/Mover.h"

namespace connector::record {
	
	/*!
	 Wrapper to box/unbox model cards during (de)serialisation
	 
	 Model cards are polymorphic - this class ensures the type information is included when a card is serialised
	 and the correct object type is constructed on deserialisation
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
			///The handler for model card packages
		static std::shared_ptr<active::serialise::Handler> m_handler;
	};

}
	
#endif //CONNECTOR_MODEL_CARD_MOVER
