#ifndef CONNECTOR_INTERFACE_BRIDGE_CONVERSION_RESULT
#define CONNECTOR_INTERFACE_BRIDGE_CONVERSION_RESULT

#include "Active/Serialise/Package/Package.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Connector/Interface/Browser/Bridge/Send/Arg/SendError.h"
#include "Speckle/Utility/String.h"

namespace connector::interfac::browser::bridge {
	
	/*!
	 The result of a conversion process (BIM records to/from Speckle)
	 */
	class ConversionResult : public active::serialise::Package {
	public:
		enum class Status : uint16_t {
			success = 1, ///<The conversion was successful
			info,	///<Not in use yet, maybe later as discussed
			warning,	///<Not in use yet, maybe later as discussed
			error,	///<An error occurred during conversion
		};
				
		// MARK: - Constructors
		
		/*!
			Default constructor
		*/
		ConversionResult() {}
				
		// MARK: - Public variables
		
			///The element conversion status
		Status status = Status::info;
			///For receive conversion reports, this is the id of the speckle object. For send, it's the host app object id.
		speckle::utility::String sourceID;
			///For receive conversion reports, this is the type of the speckle object. For send, it's the host app object type.
		speckle::utility::String sourceType;
			///For receive conversion reports, this is the id of the host app object. For send, it's the speckle object id.
		speckle::utility::String resultID;
			///For receive conversion reports, this is the type of the host app object. For send, it's the speckle object type.
		speckle::utility::String resultType;
			///The exception (nullopt = no exception)
		SendError::Option error;

		// MARK: - Serialisation
		
		/*!
		 Fill an inventory with the package items
		 @param inventory The inventory to receive the package items
		 @return True if the package has added items to the inventory
		 */
		bool fillInventory(active::serialise::Inventory& inventory) const override;
		/*!
		 Get the specified cargo
		 @param item The inventory item to retrieve
		 @return The requested cargo (nullptr on failure)
		 */
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
	};

	/*!
	 Get a conversion status enumerator from text
	 @param text The incoming text
	 @return The equivalent conversion status (nullopt on failure)
	 */
	std::optional<ConversionResult::Status> toConversionStatus(const speckle::utility::String& text);
	/*!
	 Get the text for a conversion status value
	 @param status The incoming status
	 @return The conversion status as text
	 */
	speckle::utility::String fromConversionStatus(ConversionResult::Status status);
}

namespace active::serialise {

	/*!
	 Import the object from the specified string (specialisation for bool)
	 @param source The string to read
	 @return True if the data was successfully read
	 */
	template<> inline
	bool ValueWrap<connector::interfac::browser::bridge::ConversionResult::Status>::read(const utility::String& source) {
		auto converted = connector::interfac::browser::bridge::toConversionStatus(source);
		if (!converted)
			return false;
		base::get() = *converted;
		return true;
	} //ValueWrap<bool>::read
	

	/*!
	 Import the object from the specified string (specialisation for bool)
	 @param source The string to read
	 @return True if the data was successfully read
	 */
	template<> inline
	bool ValueWrap<std::optional<connector::interfac::browser::bridge::ConversionResult::Status>>::read(const utility::String& source) {
		auto converted = connector::interfac::browser::bridge::toConversionStatus(source);
		base::get() = converted;
		return converted.operator bool();
	} //ValueWrap<bool>::read

	
	/*!
	 Export the object to the specified string (specialisation for bool)
	 @param dest The string to write the data to
	 @return True if the data was successfully written
	 */
	template<> inline
	bool ValueWrap<connector::interfac::browser::bridge::ConversionResult::Status>::write(utility::String& dest) const {
		dest = connector::interfac::browser::bridge::fromConversionStatus(get());
		return true;
	} //ValueWrap<bool>::write
}

#endif	//CONNECTOR_INTERFACE_BRIDGE_CONVERSION_RESULT
