#ifndef CONNECTOR_INTERFACE_BRIDGE_SELECTION_CHANGE_HANDLER
#define CONNECTOR_INTERFACE_BRIDGE_SELECTION_CHANGE_HANDLER

#include "Speckle/Event/Subscriber/SelectionSubscriber.h"
#include "Speckle/Event/Type/SelectionEvent.h"

namespace connector::interfac::browser::bridge {

	/*!
	 Configuration settings class
	 */
	class SelectionChangeHandler : public speckle::event::SelectionSubscriber {
	public:
		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SelectionChangeHandler();


	protected:
		bool handle(const speckle::event::SelectionEvent& event) override;
	};

}

#endif	//CONNECTOR_INTERFACE_BRIDGE_SELECTION_CHANGE_HANDLER
