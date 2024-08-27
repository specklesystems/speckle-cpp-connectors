#ifndef SPECKLE_INTERFACE_PLATFORM_BINDING
#define SPECKLE_INTERFACE_PLATFORM_BINDING

#include "Speckle/Interface/Browser/JSBinding.h"

#ifdef ARCHICAD
#include "Speckle/Serialise/JSBase/JSBaseTransport.h"

#include <Ref.hpp>
#include <JSValues.hpp>
#else
#include "Active/Serialise/JSON/JSONTransport.h"
#endif

namespace speckle::interfac::browser {
	
		///Define the bridge binding protocols according to platform (argument/result serialisation and transport for (de)serialisation)
#ifdef ARCHICAD
		//Archicad uses a proprietary JS::Base type for JS values
	using PlatformBinding = JSBinding<GS::Ref<JS::Base>, speckle::serialise::jsbase::JSBaseTransport>;
#else
		//All others will use JSON (in a String)
	using PlatformBinding = JSBinding<String, JSONTransport>;
#endif
	
}

#endif //SPECKLE_INTERFACE_PLATFORM_BINDING
