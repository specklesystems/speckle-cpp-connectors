#include "Connector/Interface/ConnectorPalette.h"

#include "Active/Event/Event.h"
#include "Active/Utility/String.h"
#include "Active/Serialise/JSON/JSONTransport.h"
#include "Active/Utility/BufferOut.h"
#include "Connector/Connector.h"
#include "Connector/ConnectorResource.h"
#include "Connector/Event/ConnectorEventID.h"
#include "Connector/Interface/Browser/Bridge/Account/AccountBridge.h"
#include "Connector/Interface/Browser/Bridge/Base/BaseBridge.h"
#include "Connector/Interface/Browser/Bridge/Config/ConfigBridge.h"
#include "Connector/Interface/Browser/Bridge/Send/SendBridge.h"
#include "Connector/Interface/Browser/Bridge/Selection/SelectionBridge.h"
#include "Connector/Interface/Browser/Bridge/Test/TestBridge.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Event/Type/MenuEvent.h"
#include "Speckle/Interface/Browser/JSPortal.h"

#include <ACAPinc.h>
#include <DGModule.hpp>
#include <DGBrowser.hpp>

using namespace active::environment;
using namespace active::event;
using namespace connector;
using namespace speckle::interfac::browser;
using namespace connector::interfac::browser::bridge;
using namespace speckle::event;


	//NB: Following is placeholder from GS example code - will be refactored to better suit our purposes


#define BrowserPaletteResId 32500
#define BrowserPaletteMenuResId 32500
#define BrowserPaletteMenuItemIndex 1

namespace {
	
	static const GS::Guid paletteGuid("{FEE27B6B-3873-4834-98B5-F0081AA4CD45}");

	// --- Class declaration: BrowserPalette ------------------------------------------

	class BrowserPalette final : public DG::Palette, public DG::PanelObserver, public JSPortal<> {
	public:
		enum SelectionModification { RemoveFromSelection, AddToSelection };

		struct ElementInfo {
			GS::UniString guidStr;
			GS::UniString typeName;
			GS::UniString elemID;
		};

		BrowserPalette();
		virtual ~BrowserPalette();

		static bool HasInstance();
		static void CreateInstance();
		static BrowserPalette& GetInstance();
		static void DestroyInstance();

		void Show();
		void Hide();

		static GSErrCode RegisterPaletteControlCallBack();

	protected:
		enum {
			BrowserId = 1
		};

		std::shared_ptr<DG::Browser> browser;

		void InitBrowserControl();
		void SetMenuItemCheckedState(bool);

		virtual void PanelResized(const DG::PanelResizeEvent& ev) override;
		virtual	void PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted) override;

		static GSErrCode __ACENV_CALL	PaletteControlCallBack(Int32 paletteId, API_PaletteMessageID messageID, GS::IntPtr param);

		static GS::Ref<BrowserPalette> instance;
#ifdef ARCHICAD
		/*!
		 Get the portal Javascript engine
		 @return The portal Javascript engine
		 */
		std::shared_ptr<JavascriptEngine> getJSEngine() const override { return browser; }
#endif
	};

	GS::Ref<BrowserPalette>	BrowserPalette::instance;

}


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ConnectorPalette::ConnectorPalette() {
} //ConnectorPalette::ConnectorPalette


/*--------------------------------------------------------------------
	Get the event subscription list
 
	return: The subscription list (an empty list will put the subscriber into a suspended state)
  --------------------------------------------------------------------*/
ConnectorPalette::Subscription ConnectorPalette::subscription() const {
	return { {toggleConnectorPaletteID} };
} //ConnectorPalette::subscription


/*--------------------------------------------------------------------
	Start the participant operation
 
	return: True if the participant is able to continue
  --------------------------------------------------------------------*/
bool ConnectorPalette::start() {
	return (BrowserPalette::RegisterPaletteControlCallBack() == NoError);
} //ConnectorPalette::start


/*--------------------------------------------------------------------
	Receive a subscribed event
 
	event: The incoming event
 
	return: True if the event should be closed
  --------------------------------------------------------------------*/
bool ConnectorPalette::receive(const active::event::Event& event) {
	if (BrowserPalette::HasInstance() && BrowserPalette::GetInstance().IsVisible()) {
		BrowserPalette::GetInstance().Hide ();
	} else {
		if (!BrowserPalette::HasInstance())
			BrowserPalette::CreateInstance();
		BrowserPalette::GetInstance().Show();
	}
	return true;
} //ConnectorPalette::receive


	//NB: Following is placeholder from GS example code - will be refactored to better suit our purposes


// -----------------------------------------------------------------------------
// Project event handler function
// -----------------------------------------------------------------------------
static GSErrCode __ACENV_CALL NotificationHandler(API_NotifyEventID notifID, Int32 /*param*/) {
	switch(notifID) {
		case APINotify_Quit:
			BrowserPalette::DestroyInstance();
			break;
	}

	return NoError;
}   // NotificationHandler


BrowserPalette::BrowserPalette() :
	DG::Palette(ACAPI_GetOwnResModule(), BrowserPaletteResId, ACAPI_GetOwnResModule(), paletteGuid) {
	browser = std::make_shared<DG::Browser>(GetReference(), BrowserId);
	ACAPI_ProjectOperation_CatchProjectEvent(APINotify_Quit, NotificationHandler);
	Attach(*this);
	BeginEventProcessing();
		//Install required connector bridges
	install<AccountBridge>();

	if (auto ref = install<BaseBridge>(); ref) {
		if (auto baseBridgeRef = std::dynamic_pointer_cast<BaseBridge>(ref); baseBridgeRef) {
			connector::connector()->addWeak(baseBridgeRef);
			baseBridgeRef->start();
		}
	}

	install<ConfigBridge>();
	install<SendBridge>();

	if (auto ref = install<SelectionBridge>(); ref) {
		if (auto selectionBridgeRef = std::dynamic_pointer_cast<SelectionBridge>(ref); selectionBridgeRef) {
			connector::connector()->addWeak(selectionBridgeRef);
			selectionBridgeRef->start();
		}
	}

	install<TestBridge>();
	InitBrowserControl();
}

BrowserPalette::~BrowserPalette() {
	EndEventProcessing();
}

bool BrowserPalette::HasInstance() {
	return instance != nullptr;
}

void BrowserPalette::CreateInstance() {
	DBASSERT(!HasInstance());
	instance = new BrowserPalette();
	ACAPI_KeepInMemory(true);
}

BrowserPalette&	BrowserPalette::GetInstance() {
	DBASSERT(HasInstance());
	return *instance;
}

void BrowserPalette::DestroyInstance() {
	instance = nullptr;
}

void BrowserPalette::Show() {
	DG::Palette::Show();
	SetMenuItemCheckedState(true);
}

void BrowserPalette::Hide() {
	DG::Palette::Hide();
	SetMenuItemCheckedState(false);
}

void BrowserPalette::InitBrowserControl() {
#ifdef TESTING_MODE
	browser->LoadURL("https://deploy-preview-3180--boisterous-douhua-e3cefb.netlify.app/");
	//browser->LoadURL("https://boisterous-douhua-e3cefb.netlify.app/test");
#else
	browser->LoadURL("https://boisterous-douhua-e3cefb.netlify.app/");
#endif
}


void BrowserPalette::SetMenuItemCheckedState(bool isChecked) {
	API_MenuItemRef	itemRef = {};
	GSFlags			itemFlags = {};

	itemRef.menuResID = BrowserPaletteMenuResId;
	itemRef.itemIndex = BrowserPaletteMenuItemIndex;

	ACAPI_MenuItem_GetMenuItemFlags(&itemRef, &itemFlags);
	if(isChecked)
		itemFlags |= API_MenuItemChecked;
	else
		itemFlags &= ~API_MenuItemChecked;
	ACAPI_MenuItem_SetMenuItemFlags(&itemRef, &itemFlags);
}

void BrowserPalette::PanelResized(const DG::PanelResizeEvent& ev) {
	BeginMoveResizeItems();
	browser->Resize(ev.GetHorizontalChange(), ev.GetVerticalChange());
	EndMoveResizeItems();
}

void BrowserPalette::PanelCloseRequested(const DG::PanelCloseRequestEvent&, bool* accepted) {
	Hide();
	*accepted = true;
}

GSErrCode __ACENV_CALL	BrowserPalette::PaletteControlCallBack(Int32, API_PaletteMessageID messageID, GS::IntPtr param) {
	switch(messageID) {
		case APIPalMsg_OpenPalette:
			if(!HasInstance())
				CreateInstance();
			GetInstance().Show();
			break;

		case APIPalMsg_ClosePalette:
			if(!HasInstance())
				break;
			GetInstance().Hide();
			break;

		case APIPalMsg_HidePalette_Begin:
			if(HasInstance() && GetInstance().IsVisible())
				GetInstance().Hide();
			break;

		case APIPalMsg_HidePalette_End:
			if(HasInstance() && !GetInstance().IsVisible())
				GetInstance().Show();
			break;

		case APIPalMsg_DisableItems_Begin:
			if(HasInstance() && GetInstance().IsVisible())
				GetInstance().DisableItems();
			break;

		case APIPalMsg_DisableItems_End:
			if(HasInstance() && GetInstance().IsVisible())
				GetInstance().EnableItems();
			break;

		case APIPalMsg_IsPaletteVisible:
			*(reinterpret_cast<bool*>(param)) = HasInstance() && GetInstance().IsVisible();
			break;

		default:
			break;
	}

	return NoError;
}

GSErrCode BrowserPalette::RegisterPaletteControlCallBack() {
	return ACAPI_RegisterModelessWindow(
					GS::CalculateHashValue(paletteGuid),
					PaletteControlCallBack,
					API_PalEnabled_FloorPlan + API_PalEnabled_Section + API_PalEnabled_Elevation +
					API_PalEnabled_InteriorElevation + API_PalEnabled_3D + API_PalEnabled_Detail +
					API_PalEnabled_Worksheet + API_PalEnabled_Layout + API_PalEnabled_DocumentFrom3D,
					GSGuid2APIGuid(paletteGuid));
}
