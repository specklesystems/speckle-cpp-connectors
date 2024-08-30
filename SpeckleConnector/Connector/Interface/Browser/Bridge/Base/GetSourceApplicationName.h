#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_APP_NAME
#define CONNECTOR_INTERFACE_BRIDGE_GET_APP_NAME

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to retrieve the host application name
	*/
	class GetSourceApplicationName : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GetSourceApplicationName();

		// MARK: - Functions (const)
		
		/*!
		 Get the host application name
		 @return The application name
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_APP_NAME
