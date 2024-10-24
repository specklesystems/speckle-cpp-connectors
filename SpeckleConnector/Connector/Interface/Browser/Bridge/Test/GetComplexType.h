#ifndef CONNECTOR_INTERFACE_BRIDGE_GET_COMPLEX_TYPE
#define CONNECTOR_INTERFACE_BRIDGE_GET_COMPLEX_TYPE

#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {

	/*!
	 JS Function class to return a specific object type
	*/
	class GetComplexType : public speckle::interfac::browser::bridge::BridgeMethod<void, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
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

#endif	//CONNECTOR_INTERFACE_BRIDGE_GET_COMPLEX_TYPE
