# Connector

An Archicad add-on to exchange models with Speckle 

## Installation

### Requirements

**Mac Platforms**

- Xcode 15.0 or higher

**Windows Platforms**

- Visual Studio 2019 (preferably 2022)

## Dependencies

This package is dependent on the following SDKs/libraries - these should be named as follows in the same directory as speckle-cpp-connectors checkout (SpeckleConnector/../../):
- "ActiveLib": <https://github.com/r-wessel/ActiveLib>

- "Archicad 27": <https://github.com/GRAPHISOFT/archicad-api-devkit/releases/tag/27.3001>

The package is also dependent on "SpeckleLib", which is included in the *speckle-cpp-connectors* repository.

## Building

The build process will automatically install the add-on if Archicad 27 is installed, but expects to find it at the standard path:
- macOS: */Applications/Graphisoft/Archicad 27*
- Win: *C:\Program Files\Graphisoft\Archicad 27*

It may be necessary to permit write access to these directories to allow the build script to copy the necessary file(s).

Otherwise, the add-on can be installed manually.

**Mac Platforms**

- Open Connector.xcworkspace in Xcode
- Pick the scheme Connector-AC27-EN_GB
- Click the menu Product > Build

Manual installation (where necessary):

From: *Connector/Install/Connector/EN-GB/ConnectorMac64-Archicad27-EN-GB/Speckle Connector.bundle*

To: */Applications/Graphisoft/Archicad 27/Add-Ons*

**Windows Platforms**

- Open Connector.sln in Visual Studio
- Pick the target ReleaseAC27
- Click the menu Build > Build Solution

Manual installation (where necessary):

From: *Connector\Install\Connector\EN-GB\ConnectorWin64-Archicad27-EN-GB\Connector.apx*

To: *C:\Program Files\Graphisoft\Archicad 27\Add-Ons*


## Usage

- Open Archicad 27
- Confirm the add-on is installed by clicking the menu item *Options > Add-On Manager* (look for "Speckle Connector" in the list - an exclamation mark denotes a problem)
- Click the menu item *Window > Palettes > Speckle Connector*
