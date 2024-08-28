#include "Speckle/Serialise/JSBase/JSBaseTransport.h"

#include "Active/Serialise/Item/Item.h"
#include "Active/Serialise/Item/Wrapper/AnyValueWrap.h"
#include "Active/Serialise/Null.h"
#include "Active/Setting/Values/BoolValue.h"
#include "Active/Setting/Values/DoubleValue.h"
#include "Active/Setting/Values/Int32Value.h"
#include "Active/Setting/Values/StringValue.h"
#include "Active/Setting/Values/UInt32Value.h"
#include "Active/Serialise/Package/Package.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Active/Serialise/XML/Item/XMLDateTime.h"

#include <JSON/JDOMWriter.hpp>
#include <JSON/Value.hpp>

using namespace active::serialise;
using namespace active::setting;
using namespace speckle::serialise;
using namespace speckle::serialise::jsbase;
using namespace speckle::utility;

using enum JSBaseTransport::Status;

namespace {

	///Category for JSBase processing errors
	class JSBaseCategory : public std::error_category {
	public:
		///Category name
		const char* name() const noexcept override {
			return "speckle::serialise::jsbase::category";
		}
		/*!
			Get a message for a specified error code
			@param errorCode A JSBase processing code
			@return The error message for the specified code
		*/
		std::string message(int errorCode) const override {
			switch (static_cast<JSBaseTransport::Status>(errorCode)) {
				case nominal:
					return "";
				case badEncoding:
					return "Found an escaped character with invalid encoding";
				case badSource:
					return "The JSBase source failed, e.g. corrupt file";
				case nameMissing:
					return "Found an object with no name";
				case badValue:
					return "An invalid value was found";
				case badDestination:
					return "The JSBase write destination failed";
				case missingInventory:
					return "An object to be sent/received via JSBase cannot provide a content inventory";
				case inventoryBoundsExceeded:
					return "Found more instances of a named value/object than the inventory permits";
				case invalidObject:
					return "An invalid object instance was found";
				case unknownName:
					return "An unknown name was found in the JSBase";
				default:
					return "Unknown/invalid error";
			}
		}
	};


	///JSBase processing category error instance
	static JSBaseCategory instance;


	///Make an error code for JSBase processing
	inline std::error_code makeJSBaseError(JSBaseTransport::Status code) {
		return std::error_code(static_cast<int>(code), instance);
	}


	///Identification type for JSBase elements
	struct JSBaseIdentity : Identity {

		// MARK: Types

			///Enumeration of JSBase element tag types
		enum class Type {
			undefined,		///<No type identified
			objectStart,	///<Object start brace, i.e. {
			arrayStart,		///<Array start brace, [
			valueStart,		///<An item value, e.g. "Ralph"
			objectEnd,		///<Object end brace, i.e. }
			arrayEnd,		///<Array end brace, i.e. ]
		};

		///Enumeration of JSBase parsing stages
		enum class Stage {
			root,	///<A new element is expected, either a new object, array or (unnamed) value
			array,	///<Within an array - same as root condition, but different terminator expected
			object,	///<Within an object - a named value is expected
			complete,	///<An element has been read
		};

		// MARK: Constructors

		/*!
			Default constructor
			@param identity The element identity
			@param tagType The tag type
		*/
		JSBaseIdentity(const Identity& identity = Identity(), Type tagType = Type::undefined) : Identity(identity) {
			if (const auto* jsonIdentity = dynamic_cast<const JSBaseIdentity*>(&identity); jsonIdentity != nullptr) {
				type = jsonIdentity->type;
				stage = jsonIdentity->stage;
			}
			else
				type = tagType;
		}
		/*!
			Constructor
			@param tagType The tag type
		*/
		JSBaseIdentity(Type tagType) : Identity() { type = tagType; }

		// MARK: Public variables

			///The element type
		Type type = Type::undefined;
		///The stage at which the identity is found
		Stage stage = Stage::root;

		// MARK: Functions (mutating)

		/*!
			Set the identity tag as the hierarchy root
			@return A reference to this
		*/
		JSBaseIdentity& atStage(Stage newStage) {
			stage = newStage;
			return *this;
		}

		/*!
			Set the identity tag type
			@param tagType The tag type
			@return A reference to this
		*/
		JSBaseIdentity& withType(Type tagType) {
			type = tagType;
			return *this;
		}
	};

	using JSElements = std::vector<std::pair<JS::Base*, String::Option>>;

	using enum JSBaseIdentity::Type;
	using enum JSBaseIdentity::Stage;


	/*--------------------------------------------------------------------
		Add an item to a JSBase object

		item: The item to write
		destination: The JSBase destination
	  --------------------------------------------------------------------*/
	void addJSBase(GS::Ref<JS::Base>& item, const String& tag, GS::Ref<JS::Base>& destination) {
			//Attempt to add to object
		if (auto object = dynamic_cast<JS::Object*>(destination.operator JS::Base * ()); object != nullptr)
			object->AddItem(tag, item);
			//Attempt to add to array
		else if (auto array = dynamic_cast<JS::Array*>(destination.operator JS::Base * ()); array != nullptr)
			array->AddItem(item);
		else
			throw std::system_error(makeJSBaseError(badDestination));	//The destination isn't a container
		return;
	} //addJSBase


	/*--------------------------------------------------------------------
		Write an item to a JSBase object

		item: The item to write
		tag: The item tag
		destination: The JSBase destination
	  --------------------------------------------------------------------*/
	void writeValue(const Item& item, const String& tag, GS::Ref<JS::Base>& destination) {
		GS::Ref<JS::Base> newValue;
		switch (item.type().value_or(Item::text)) {
			case Item::boolean: {
				BoolValue value;
				if (!item.write(value))
					throw std::system_error(makeJSBaseError(badValue));
				newValue = new JS::Value(value.operator bool());
				break;
			}
			case Item::number: {
				DoubleValue value;
				if (!item.write(value))
					throw std::system_error(makeJSBaseError(badValue));
				newValue = new JS::Value(value.operator double());
				break;
			}
			case Item::text: {
				String value;
				if (!item.write(value))
					throw std::system_error(makeJSBaseError(badValue));
				newValue = new JS::Value(value);
				break;
			}
		}
		if (destination)
			addJSBase(newValue, tag, destination);
		else
			destination = newValue;
	} //writeValue


	/*--------------------------------------------------------------------
		Write a null value to a JSBase object

		tag: The item tag
		destination: The JSBase destination
	  --------------------------------------------------------------------*/
	void writeNull(const String& tag, GS::Ref<JS::Base>& destination) {
		GS::Ref<JS::Base> newValue = new JS::Value{};
		if (destination)
			addJSBase(newValue, tag, destination);
		else
			destination = newValue;
	} //writeNull


	/*--------------------------------------------------------------------
		Decompose a JSBase into constitient items, paired with a name where possible

		source: The source JSBase

		return: The items in the JSBase
	  --------------------------------------------------------------------*/
	JSElements decomposeJSBase(JS::Base& source) {
		JSElements result;
		if (auto object = dynamic_cast<JS::Object*>(&source); object != nullptr) {
				//Decompose an object
			for (auto& item : object->GetItemTable())
				result.push_back({ item.value->operator JS::Base * (), String{*item.key} });
		}
		else if (auto array = dynamic_cast<JS::Array*>(&source); array != nullptr) {
				//Decompose an array
			for (auto& item : array->GetItemArray())
				result.push_back({ item, std::nullopt });
		}
		else
			throw std::system_error(makeJSBaseError(badSource));	//The source isn't a container
		return result;
	} //decomposeJSBase


	/*--------------------------------------------------------------------
		Import a cargo item from a JSBase element

		cargo: A cargo item to import the phrase
		source: The JSBase element to be imported
	  --------------------------------------------------------------------*/
	void readValue(Cargo& cargo, JS::Base& source) {
		auto* item = dynamic_cast<Item*>(&cargo);
		if (item == nullptr)
			throw std::system_error(makeJSBaseError(badValue));
		auto value = dynamic_cast<JS::Value*>(&source);
		if (value == nullptr)
			throw std::system_error(makeJSBaseError(badSource));	//The source isn't a value
		switch (value->GetType()) {
			case JS::Value::ValueType::BOOL:
				item->read(BoolValue{value->GetBool()});
				break;
			case JS::Value::ValueType::INTEGER:
				item->read(Int32Value{value->GetInteger()});
				break;
			case JS::Value::ValueType::UINTEGER:
				item->read(UInt32Value{value->GetUInteger()});
				break;
			case JS::Value::ValueType::DOUBLE:
				item->read(DoubleValue{value->GetDouble()});
				break;
			case JS::Value::ValueType::STRING:
				item->read(StringValue{String{value->GetString()}});
				break;
			default:
				break;
		}
	} //doJSBaseItemImport


	/*--------------------------------------------------------------------
		Import the contents of the specified cargo from JSBase

		container: The cargo container to receive the imported data
		containerIdentity: The container identity
		source: The JSBase source
	  --------------------------------------------------------------------*/
	void doJSBaseImport(Cargo& container, const JSBaseIdentity& containerIdentity, JS::Base& source) {
		if (dynamic_cast<Item*>(&container) != nullptr) {
			//If we've got a single-value item at the root, import the source value and end
			readValue(container, source);
			return;
		}
			//Find out what the container can hold
		Inventory inventory;
		if (!container.fillInventory(inventory))
			throw std::system_error(makeJSBaseError(missingInventory));
		inventory.resetAvailable();	//Reset the availability of each entry to zero so we can count incoming items
		auto elements = decomposeJSBase(source);
		if (elements.empty())
			return;
		bool isArray = !elements[0].second;
		Identity parentIdentity{ containerIdentity };
			//Anonymous arrays need an identity
		if (isArray && parentIdentity.name.empty()) {
			for (auto& entry : inventory)
				if (entry.isRepeating())
					parentIdentity = entry.identity();
			if (parentIdentity.name.empty())
				throw std::system_error(makeJSBaseError(invalidObject));
		}
		for (auto& element : elements) {
			Cargo::Unique cargo;
			Inventory::iterator incomingItem = inventory.end();
			Identity identity{ element.second.value_or(parentIdentity.name) };
			if (incomingItem = inventory.registerIncoming(identity); incomingItem != inventory.end()) {	//Seek the incoming element in the inventory
				if (!incomingItem->bumpAvailable())
					throw std::system_error(makeJSBaseError(inventoryBoundsExceeded));
				cargo = container.getCargo(*incomingItem);
			}
			if (!cargo)
				continue;	//TODO: Add option to throw exception for unknown elements
			cargo->setDefault();
			doJSBaseImport(*cargo, identity, *element.first);
			if (incomingItem->isRepeating()) {
				if (auto package = dynamic_cast<Package*>(&container);
					(package != nullptr) && !package->insert(std::move(cargo), *incomingItem))
					throw std::system_error(makeJSBaseError(invalidObject));
			}
		}
		if (!container.validate())
			throw std::system_error(makeJSBaseError(invalidObject));	//The incoming data was rejected as invalid
	} //doJSBaseImport


	/*--------------------------------------------------------------------
		Export cargo to JSBase

		cargo: The cargo to export
		identity: The cargo identity
		destination: The JSBase destination
	  --------------------------------------------------------------------*/
	void doJSBaseExport(const Cargo& cargo, const JSBaseIdentity& identity, GS::Ref<JS::Base>& destination) {
		const auto* item = dynamic_cast<const Item*>(&cargo);
		Inventory inventory;
			//Single-value items won't specify an inventory (no point)
		if (!cargo.fillInventory(inventory) || (inventory.empty())) {
			if (item == nullptr) {
				if (dynamic_cast<const Null*>(&cargo) == nullptr)
					throw std::system_error(makeJSBaseError(badValue));	//Non-items must be named
				writeNull(identity.name, destination);
				return;
			}
			writeValue(*item, identity.name, destination);
			return;
		}
			//Determine if this cargo is a wrapper for other cargo, i.e. an object/array
		bool isWrapperTag = true;
		if (item != nullptr) {
			if (inventory.size() != 1)
				throw std::system_error(makeJSBaseError(badValue));
				//Items only act as a wrapper when different (non-empty) names are defined by the inventory and the item identity
			isWrapperTag = !identity.name.empty() && !inventory.begin()->identity().name.empty() && (inventory.begin()->identity() != identity);
		}
		auto sequence = inventory.sequence();
		auto container = destination;
		if (isWrapperTag) {
			auto containerType = cargo.entryType().value_or((inventory.size() == 1) && !(inventory.begin()->maximum() == 1) ?
				Entry::Type::array : Entry::Type::element);
			if (containerType == Entry::Type::array)
				container = new JS::Array();
			else
				container = new JS::Object();
			if (destination)
				addJSBase(container, identity.name, destination);
			else
				destination = container;
		}
		for (auto& entry : sequence) {
			auto item = *entry.second;
			if (!item.required)
				continue;
				//Each cargo container may contain multiple export items
			auto limit = item.available;
			for (item.available = 0; item.available < limit; ++item.available) {
				if (auto content = cargo.getCargo(item); content) {
					doJSBaseExport(*content, item.identity(), container);
				}
				else
					break;	//Discontinue an inventory item when the supply runs out
			}
		}
	} //doJSBaseExport


	/*--------------------------------------------------------------------
		Convert a JS::Base object to JSON

		jsBase: The object to convert
	 --------------------------------------------------------------------*/
	JSON::ValueRef convertToJSONValue(const GS::Ref<JS::Base>& jsBase) {
		JS::Object* objectJS = dynamic_cast<JS::Object*> ((JS::Base*)jsBase);
		if (objectJS != nullptr) {
			JSON::ObjectValueRef objectJSON = new JSON::ObjectValue();
			for (const auto& member : objectJS->GetItemTable())
				objectJSON->AddValue(*member.key, convertToJSONValue(*member.value));
			return objectJSON;
		}
		JS::Array* arrayJs = dynamic_cast<JS::Array*> ((JS::Base*)jsBase);
		if (arrayJs != nullptr) {
			JSON::ArrayValueRef arrayJSON = new JSON::ArrayValue();
			for (const auto& item : arrayJs->GetItemArray())
				arrayJSON->AddValue(convertToJSONValue(item));
			return arrayJSON;
		}
		JS::Value* valueJs = dynamic_cast<JS::Value*> ((JS::Base*)jsBase);
		if (valueJs != nullptr) {
			JSON::ValueRef primitiveJSON;
			switch (valueJs->GetType()) {
				case JS::Value::DEFAULT:
					primitiveJSON = new JSON::NullValue();
					break;
				case JS::Value::BOOL:
					primitiveJSON = new JSON::BoolValue(valueJs->GetBool());
					break;
				case JS::Value::INTEGER:
					primitiveJSON = new JSON::NumberValue(valueJs->GetInteger());
					break;
				case JS::Value::UINTEGER:
					primitiveJSON = new JSON::NumberValue(valueJs->GetUInteger());
					break;
				case JS::Value::DOUBLE:
					primitiveJSON = new JSON::NumberValue(valueJs->GetDouble());
					break;
				case JS::Value::STRING:
					primitiveJSON = new JSON::StringValue(valueJs->GetString());
					break;
				default:
					DBBREAK();
			}
			return primitiveJSON;
		}
		return nullptr;
	} //convertToJSONValue

}

/*--------------------------------------------------------------------
	Send cargo as JSBase to a specified destination

	cargo: The cargo to be sent as JS::Base
	identity: The cargo identity (name, optional namespace)
	destination: A reference to a JS::Base object (will be populated by this function)
  --------------------------------------------------------------------*/
void JSBaseTransport::send(Cargo&& cargo, const Identity& identity, GS::Ref<JS::Base>& destination) const {
	doJSBaseExport(cargo, JSBaseIdentity(identity).atStage(root), destination);
	OutputDebugString((WCHAR*)String {"\nSent:\n" + convertToJSON(destination)}.operator std::u16string().data());
} //JSBaseTransport::send


/*--------------------------------------------------------------------
	Receive cargo from a specified JSBase source

	cargo: The cargo to receive the JS::Base data
	identity: The cargo identity (name, optional namespace)
	source: A reference to a JS::Base object
  --------------------------------------------------------------------*/
void JSBaseTransport::receive(Cargo&& cargo, const Identity& identity, GS::Ref<JS::Base> source) const {
	if (!source)
		throw std::system_error(makeJSBaseError(badSource));
	OutputDebugString((WCHAR*) String{"\nReceived:\n" + convertToJSON(source)}.operator std::u16string().data());
	doJSBaseImport(cargo, JSBaseIdentity(identity).atStage(root), *source);
} //JSBaseTransport::receive


/*--------------------------------------------------------------------
	Convert a JS::Base object to JSON

	jsBase: The object to convert
  --------------------------------------------------------------------*/
String JSBaseTransport::convertToJSON(const GS::Ref<JS::Base>& jsBase) {
	GS::UniString resultString;
	try {
			//JDOMStringWriter can't cope with single values
		if (auto jsValue = dynamic_cast<const JS::Value*>(jsBase.operator JS::Base * ()); jsValue != nullptr) {
			AnyValueWrap value;
			readValue(value, *jsBase);
			String json;
			value.write(json);
			return json;
		} else if (auto jsonRef = convertToJSONValue(jsBase); jsonRef != nullptr)
			JSON::JDOMStringWriter writer(*jsonRef, resultString);
	} catch (...) {}
	return resultString;
} //JSBaseTransport::convertToJSON
