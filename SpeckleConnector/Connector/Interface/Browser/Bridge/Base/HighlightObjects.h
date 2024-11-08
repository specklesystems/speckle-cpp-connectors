#ifndef CONNECTOR_INTERFACE_BRIDGE_HIGHLIGHT_OBJECTS
#define CONNECTOR_INTERFACE_BRIDGE_HIGHLIGHT_OBJECTS

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Interface/Browser/Bridge/BridgeMethod.h"

namespace connector::interfac::browser::bridge {
	
		///List of strings
	using StringList = std::vector<active::utility::String>;
		///Argument parameter for a string
	using StringListHold = active::serialise::CargoHold<active::serialise::ContainerWrap<StringList>, StringList>;
		///Argument type for this method
	using SendArgs = speckle::interfac::browser::bridge::JSArgType<StringListHold>;

	/*!
	 JS Function class to highlight objects using passed application IDs
	*/
	class HighlightObjects : public speckle::interfac::browser::bridge::BridgeMethod<SendArgs, void> {
	public:

		// MARK: - Constructors
		
		/*!
		 Constructor
		 */
		HighlightObjects();

		// MARK: - Functions (const)
		
		/*!
		 Highlight specified objects
		 @param objectIDs List of object IDs to be highlighted
		 */
		void run(const StringList& objectIDs) const;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_HIGHLIGHT_OBJECTS
