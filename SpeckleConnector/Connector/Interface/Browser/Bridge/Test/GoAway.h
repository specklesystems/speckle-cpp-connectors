#ifndef CONNECTOR_INTERFACE_BRIDGE_GO_AWAY
#define CONNECTOR_INTERFACE_BRIDGE_GO_AWAY

#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 JS Function class to print a fixed message to the console
	*/
	class GoAway : public speckle::interfac::browser::bridge::JSBridgeMethod<void, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		GoAway();

		// MARK: - Functions (const)
		
		/*!
		 Write a message to the console
		 */
		void run() const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_GO_AWAY
