#ifndef CONNECTOR_RESOURCE
#define CONNECTOR_RESOURCE

constexpr short connectorResourceId = 32500;

	//Addon resource ID's, e.g. menus
enum AddonResource {
	speckleMenu = 32500,
};


	//Menu item help
enum HelpString {
	speckleMenuHelp = 32550,
};

	//String resource IDs
enum StringResource {
	titleString = 32600,
	generalString,
	notifyString,
	warningString,
	errorString,
};


	//Title strings (UI title/label for dialogs, controls, menu items etc)
enum TitleString {
	addonNameID = 1,
	addonDescriptionID,
};


	//Help strings
enum PromptString {
};


	//Information strings (in UI content, logging, reports)
enum InfoString {
};


	//Notification strings (advice displayed in alerts)
enum NotifyString {
};


	//Warning strings (warnings displayed in alerts)
enum WarningString {
};


	//Error strings (errors displayed in alerts)
enum ErrorString {
};

#endif	//CONNECTOR_RESOURCE
