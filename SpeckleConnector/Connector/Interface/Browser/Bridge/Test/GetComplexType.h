#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG
#define CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG

#include "Connector/Interface/Browser/Bridge/Test/Arg/ComplexType.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {

	/*!
	 JS Function class to return a specific object type
	*/
	class GetComplexType : public speckle::interfac::browser::bridge::JSBridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param bridge The parent bridge object (provides access to bridge methods)
		 */
		GetComplexType();

		// MARK: - Functions (const)
		
		/*!
		 Get the required object type
		 @return The required object
		 */
		std::unique_ptr<active::serialise::Cargo> run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_CONFIG