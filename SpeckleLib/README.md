# SpeckleLib

Foundational, platform-independent C++ classes to support Speckle development

## Contents
1. [Scope](#scope)
2. [Installation](#install)
	- [Requirements](#require)
	- [Dependencies](#depend)
	- [Building](#build)
		- [Cmake](#cmake)
		- [Visual Studio](#vs)
		- [Xcode](#xcode)
4. Modules
	- [Environment](./Speckle/Environment/Environment.md)
	- [Event](./Speckle/Event/Event.md)
	- [Serialisation](./Speckle/Serialise/Serialisation.md)


## Scope <a name="scope"></a>

This library is designed to be cross-platform, supporting Linux, iOS, macOS and Windows (including C++/CLR contexts). The base standard is currenty C++20 to allow broad compiler support.

## Installation <a name="install"></a>

### Requirements <a name="require"></a>

Compiler must support C++20

### Dependencies <a name="depend"></a>

This package is dependent on the following SDKs/libraries - these should be named as follows in the same directory as speckle-cpp-connectors (SpeckleLib/../../):
- "ActiveLib": <https://github.com/r-wessel/ActiveLib>

### Building <a name="build"></a>

The library can be built using a variety of tools. Unit tests build and run in each context:

|  |  |
| --- | --- |
| *Cmake* | Built against [DocTest](https://github.com/doctest/doctest) |
| *Visual Studio* | Run in [Test Explorer](https://learn.microsoft.com/en-us/visualstudio/test/run-unit-tests-with-test-explorer?view=vs-2022) |
| *Xcode* | Run with [XCTest](https://developer.apple.com/documentation/xctest) |


#### Cmake <a name="cmake"></a>

The following command-line instructions will build the library and tests:

	cmake -B ./build
	cmake --build ./build --config Release

> Note: Directing the output to a *build* directory is simply a practical suggestion, not a requirement. The build output can be directed anywhere.

For Visual Studio builds, it's prudent to specify a compiler and toolset, e.g.:

	cmake -B ./build/v143 -G "Visual Studio 17" -A x64 -DCMAKE_GENERATOR_TOOLSET=v143
	cmake --build ./build/v143 --config Release

Unit tests are built into a single executable called *specklelibtests* and can be executed from the command-line, e.g.:

	./specklelibtests

#### Visual Studio <a name="vs"></a>

- Open SpeckleLib.sln in Visual Studio 2022
- Pick the target Release17
- Click the menu Build > Build Solution
- Run unit tests by clicking Test > Run All Tests

#### Xcode <a name="xcode"></a>

- Open SpeckleLib.xcworkspace in Xcode
- Pick the scheme SpeckleLib-Release
- Click the menu Product > Build
- Run unit tests by clicking Product > Test

### Documentation <a name="docs"></a>

The source contains Doxygen-style quotes for building a documentation set, which can be viewed [here](https://www.speckle.systems/SpeckleLib/Documentation/index.html).

Alternatively, the documentation can be built using the configuration file `./Documentation/Doxyfile`, either through the GUI application or from the command line: `doxygen ./Documentation/Doxyfile`

The latter will build the documentation into `./Documentation/html/`
