#ifndef CONNECTOR_INTERFACE_BRIDGE_ADD_MODEL
#define CONNECTOR_INTERFACE_BRIDGE_ADD_MODEL

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Package/Wrapper/PackageWrap.h"
#include "Connector/Database/Model/Card/ModelCard.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
		///Argument parameter for a string
	using CardHold = active::serialise::CargoHold<active::serialise::PackageWrap, connector::database::ModelCard>;
		///Argument type for this method
	using ModelCardEventWrapper = speckle::interfac::browser::bridge::JSArgType<CardHold>;

	/*!
	 JS Function class to add a model card to the document storage
	*/
	class AddModel : public speckle::interfac::browser::bridge::BridgeMethod<ModelCardEventWrapper, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		AddModel();

		// MARK: - Functions (const)
		
		/*!
		 Add a model card to document storage
		 @param card The card to add
		 */
		void run(connector::database::ModelCard& card) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_ADD_MODEL
