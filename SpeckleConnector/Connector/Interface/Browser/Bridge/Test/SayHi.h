#ifndef CONNECTOR_INTERFACE_BRIDGE_SAYHI
#define CONNECTOR_INTERFACE_BRIDGE_SAYHI

#include "Connector/Interface/Browser/Bridge/Test/Arg/SayHiArg.h"
#include "Speckle/Interface/Browser/Bridge/JSBridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
		///Argument for the parameters of a test message
	using SayHiWrapper = speckle::interfac::browser::bridge::JSArgType<SayHiArg>;

	/*!
	 JS Function class to return a test message based on sample data
	*/
	class SayHi : public speckle::interfac::browser::bridge::JSBridgeMethod<SayHiWrapper, active::serialise::Cargo> {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SayHi();

		// MARK: - Functions (const)
		
		/*!
		 Return a message based on sample data
		 @param arg The sample data
		 @return The message based on sample data
		 */
		std::unique_ptr<active::serialise::Cargo> run(SayHiArg arg) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SAYHI
