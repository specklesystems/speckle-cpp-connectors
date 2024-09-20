#ifndef CONNECTOR_INTERFACE_BRIDGE_UPDATE_MODEL
#define CONNECTOR_INTERFACE_BRIDGE_UPDATE_MODEL

#include "Active/Serialise/CargoHold.h"
#include "Connector/Record/Model/CardMover.h"
#include "Connector/Record/Model/ModelCard.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
		///Argument parameter for a string
	using CardHold = active::serialise::CargoHold<connector::record::CardMover, connector::record::ModelCard>;
		///Argument type for this method
	using ModelCardEventWrapper = speckle::interfac::browser::bridge::JSArgType<CardHold>;

	/*!
	 JS Function class to add a model card to the document storage
	*/
	class UpdateModel : public speckle::interfac::browser::bridge::BridgeMethod<ModelCardEventWrapper, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		UpdateModel();

		// MARK: - Functions (const)
		
		/*!
		 Add a model card to document storage
		 @param card The card to add
		 */
		void run(const connector::record::ModelCard& card) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_UPDATE_MODEL
