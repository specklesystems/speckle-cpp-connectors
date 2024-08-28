#ifndef SPECKLE_SERIALISE_JS_BASE_TRANSPORT
#define SPECKLE_SERIALISE_JS_BASE_TRANSPORT

#ifdef ARCHICAD	//NB: This transport is Archicad-specific

#include "Speckle/Utility/String.h"

#include <Ref.hpp>
#include <JSValues.hpp>

namespace active::serialise {
	
	class Cargo;
	struct Identity;

}

namespace speckle::serialise::jsbase {
	
	/*!
		A serialisation tool class to send/receive data serialised via JS::Base (Archicad API JavaScript value type)
	*/
	class JSBaseTransport {
	public:
		
		/*!
		 Convert a JS::Base object to JSON
		 @param jsBase The object to convert
		 */
		static speckle::utility::String convertToJSON(const GS::Ref<JS::Base>& jsBase);

		// MARK: - Types
		
			///Status of of the XML transport
		enum class Status {
			nominal,	///<No errors logged
			badEncoding,	///<The encoding of a string is invalid
			badSource,	///<The JSBase source failed, e.g. null reference
			nameMissing,	///<The name of a value/object is missing
			badValue,	///<A value was found, but invalid, e.g. text where a number was expected or a number that couldn't be parsed
			badDestination,	///<The JSBase destination failed, e.g. out of memory
			missingInventory,	///<An object has been provided to send/receive data, but it cannot provide an inventory for its content
			inventoryBoundsExceeded,	///<The source contains more items of a specific type than the inventory maximum permits
			invalidObject,	///<Insertion of an incoming object was rejected, indicating invalid content
			unknownName,	///<An unknown name was found in the JSBase source
		};
		
		// MARK: - Functions (const)
		
		/*!
			Send cargo as XML to a specified destination
			@param cargo The cargo to be sent as JS::Base
			@param identity The cargo identity (name, optional namespace)
			@param destination A reference to a JS::Base object (will be populated by this function)
			@throw std::system_error Thrown on write errors, serialisation failure (e.g. missing inventory etc)
		*/
		virtual void send(active::serialise::Cargo&& cargo, const active::serialise::Identity& identity, GS::Ref<JS::Base>& destination) const;
		/*!
			Receive cargo from a specified XML source
			@param cargo The cargo to receive the JS::Base data
			@param identity The cargo identity (name, optional namespace)
			@param source A reference to a JS::Base object
			@throw std::system_error Thrown on read errors, invalid encoding or parsing failure (e.g. bad object references)
		*/
		void receive(active::serialise::Cargo&& cargo, const active::serialise::Identity& identity, GS::Ref<JS::Base> source) const;
		/*!
			Determine if unknown values are skipped
			@return True if unknown values are skipped
		*/
		bool isUnknownValueSkipped() const noexcept { return m_isUnknownValueSkipped; }
		
		// MARK: - Functions (mutating)
		
		/*!
			Set whether unknown values are skipped
			@param state True if unknown values are skipped
		*/
		void setUnknownValueSkipped(bool state) noexcept { m_isUnknownValueSkipped = state; }
		
	private:
			//True if unknown values should be skipped over
		bool m_isUnknownValueSkipped = true;
	};
	
}

#endif	//ARCHICAD
#endif	//SPECKLE_SERIALISE_JS_BASE_TRANSPORT
