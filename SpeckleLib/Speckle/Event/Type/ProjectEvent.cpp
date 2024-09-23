#include "Speckle/Event/Type/ProjectEvent.h"

using namespace speckle::event;

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Convert an Archicad project event type
 
	return: The equivalent Speckle event type
  --------------------------------------------------------------------*/
std::optional<ProjectEvent::Type> ProjectEvent::convert(API_NotifyEventID acEventType) {
	using enum ProjectEvent::Type;
	switch (acEventType) {
		case APINotify_New:
			return newDocument;
		case APINotify_NewAndReset:
			return newAndReset;
		case APINotify_Open:
			return open;
		case APINotify_PreSave:
			return presave;
		case APINotify_Save:
			return save;
		case APINotify_Close:
			return close;
		case APINotify_Quit:
			return quit;
		case APINotify_TempSave:
			return tempSave;
		case APINotify_SendChanges:
			return send;
		case APINotify_ReceiveChanges:
			return receive;
		default:
			return std::nullopt;
	} //ProjectEvent::convert
}
#endif
