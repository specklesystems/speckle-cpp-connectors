#include "Speckle/Serialise/JSBase/JSBaseTransport.h"

#include "Active/Serialise/Item/Item.h"
#include "Active/Setting/Values/BoolValue.h"
#include "Active/Setting/Values/DoubleValue.h"
#include "Active/Serialise/Package/Package.h"
#include "Active/Serialise/Package/PackageWrap.h"
#include "Active/Serialise/XML/Item/XMLDateTime.h"

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
			root,		///<A new element is expected, either a new object, array or (unnamed) value
			array,		///<Within an array - same as root condition, but different terminator expected
			object,		///<Within an object - a named value is expected
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
			} else
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
	
	using enum JSBaseIdentity::Type;
	using enum JSBaseIdentity::Stage;
	
	
	/*--------------------------------------------------------------------
		Write an item to a JSBase object
	 
		item: The item to write
		destination: The JSBase destination
	  --------------------------------------------------------------------*/
	void writeValue(const Item& item, const String& tag, GS::Ref<JS::Base>& destination) {
		GS::Ref<JS::Base> newValue;
		switch(item.type().value_or(Item::text)) {
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
		if (destination) {
			auto object = JSON::ObjectValue::Cast(destination);
			if (object == nullptr)
				throw std::system_error(makeJSBaseError(badDestination));
			object->AddValue(tag, newValue);
			return;
		}
		destination = newValue;
	} //writeValue

	
	/*--------------------------------------------------------------------
		Import the contents of the specified cargo from JSBase
	 
		container: The cargo container to receive the imported data
		containerIdentity: The container identity
		importer: The JSBase data importer
		depth: The recursion depth into the JSBase hierarchy
	  --------------------------------------------------------------------*/
/*	void doJSBaseImport(Cargo& container, const JSBaseIdentity& containerIdentity, JSBaseImporter& importer, int32_t depth) {
		if (containerIdentity.type == valueStart) {
			if (auto* item = dynamic_cast<active::serialise::Item*>(&container); item != nullptr) {
				importer.getContent(*item);
				return;
			}
		}
		Inventory inventory = getImportInventoryFor(container);
		auto attributesRemaining = inventory.attributeSize(true);	//This is tracked where the container requires attributes first
		auto parsingStage = containerIdentity.stage;
		auto* package = dynamic_cast<Package*>(&container);
		auto isReadingAttribute = (package != nullptr) && package->isAttributeFirst();
		std::optional<Memory::size_type> restorePoint;
		for (;;) {	//We break out of this loop when an error occurs or we run out of data
			Memory::size_type readPoint = importer.getPosition();
			auto identity = importer.getIdentity(parsingStage);	//Get the identity of the next item in the JSBase source
			switch (identity.type) {
				case undefined:	//End of file
					if (depth != 0)	//Failure if tags haven't been balanced correctly
						throw std::system_error(makeJSBaseError(unbalancedScope));
					return;
				case delimiter:
					if (parsingStage != complete)	//A delimiter has been found before anything was read
						throw std::system_error(makeJSBaseError(unbalancedScope));
					parsingStage = containerIdentity.stage;
					continue;	//Move onto the next item
				case objectStart: case valueStart: case arrayStart: {
					if (parsingStage == complete)	//An element has been read, but no delimiter reached - expected a closing symbol
						throw std::system_error(makeJSBaseError(unbalancedScope));
					Cargo::Unique cargo;
					Inventory::iterator incomingItem = inventory.end();
					bool isArrayStart = ((identity.type == arrayStart) && !identity.name.empty()), isKnown = true;
					if (identity.name.empty() || isArrayStart) {
						if (identity.name.empty() && parsingStage == object)	//An element within an object must be identified with a name
							throw std::system_error(makeJSBaseError(nameMissing));
						cargo = wrapped(container);	//The next element is a child (for array) or instance (for root) of the parent container
						if (identity.name.empty()) {
							auto incomingType = identity.type;
							identity = containerIdentity;	//If no name is specified, we adopt the identity specified by the container
							identity.type = incomingType;
							if (parsingStage == root)
								identity.stage = isArrayStart ? array : object;
						}
						if (parsingStage == root)
							cargo->setDefault();	//The root object is sourced externally, so has to be reset to the default separately
					}
					if (!identity.name.empty() && (parsingStage != root) && !isArrayStart) {	//Allocate new cargo when a new element is reached
						if (incomingItem = inventory.registerIncoming(identity); incomingItem != inventory.end()) {	//Seek the incoming element in the inventory
							if (isReadingAttribute && !incomingItem->isAttribute())
								incomingItem = inventory.end();
							else {
								if (!incomingItem->bumpAvailable())
									throw std::system_error(makeJSBaseError(inventoryBoundsExceeded));
								if ((attributesRemaining > 0) && incomingItem->isAttribute() && incomingItem->required)
									--attributesRemaining;
								cargo = (incomingItem == inventory.end()) ? nullptr : container.getCargo(*incomingItem);
							}
						}
							//Allow the parser to move beyond unknown/unwanted elements
						if (!cargo) {
							if (importer.isUnknownSkipped() || isReadingAttribute) {
								isKnown = false;
								cargo = makeUnknown(identity);
								if (isReadingAttribute && !restorePoint)	//If not all attributes read, parse data twice (first for attributes only)
									restorePoint = readPoint;	//If this is the first instance, set a restore point so reading can resume here
							} else
								throw std::system_error(makeJSBaseError(unknownName));
						}
						cargo->setDefault();
					}
					doJSBaseImport(*cargo, JSBaseIdentity{identity}.atStage((identity.type == arrayStart) ? array : object), importer, depth + 1);
					if (incomingItem != inventory.end()) {
						if (incomingItem->isRepeating()) {
							if ((package != nullptr) && !package->insert(std::move(cargo), *incomingItem))
								throw std::system_error(makeJSBaseError(invalidObject));
						}
					} else if (isKnown && !isArrayStart)
						return;	//If there is no defined item, we're in an array or the root - we need to return the imported element now
					parsingStage = complete;	//An element has been parsed - we either expect a delimiter or a terminator
					break;
				}
				case objectEnd: case arrayEnd:
					if (containerIdentity.stage != (identity.type == objectEnd ? object : array))
						throw std::system_error(makeJSBaseError(unbalancedScope));	//The scope end couldn't be paired with the atart
					if (restorePoint) {
						isReadingAttribute = false;
						importer.setPosition(*restorePoint);	//Move the read position back to the first non-attribute
						restorePoint.reset();
						attributesRemaining = 0;	//It may not be an error is this is not already zero - the container will validate the result
						if (!package->finaliseAttributes())
							throw std::system_error(makeJSBaseError(invalidObject));
						inventory = getImportInventoryFor(container);	//Having finalised attributes, the container inventory will probably change
						parsingStage = object;	//Resuming reading at non-attributes is always in the context of an object
						break;
					}
					if (!container.validate())
						throw std::system_error(makeJSBaseError(invalidObject));	//The incoming data was rejected as invalid
					return;
			}
		}
	}*/ //doJSBaseImport
	
		
	/*--------------------------------------------------------------------
		Export cargo to JSBase
	
		cargo: The cargo to export
		identity: The cargo identity
		destination: The JSBase destination
	  --------------------------------------------------------------------*/
	void doJSBaseExport(const Cargo& cargo, const JSBaseIdentity& identity, GS::Ref<JS::Base>& destination) {
		String tag;
		if (identity.stage != root) {
			if (identity.name.empty())	//Non-root values, i.e. values embedded in an object, must have an identifying name
				throw std::system_error(makeJSBaseError(nameMissing));
				//Formulate and write the identifying name
			tag = identity.name;
		}
		const auto* item = dynamic_cast<const Item*>(&cargo);
		Inventory inventory;
			//Single-value items won't specify an inventory (no point)
		if (!cargo.fillInventory(inventory) || (inventory.empty())) {
			if (item == nullptr)
				throw std::system_error(makeJSBaseError(missingInventory));	//If anything other than a single-value item lands here, it's an error
			writeValue(*item, tag, destination);
			return;
		}
		if ((item != nullptr) && (inventory.size() != 1))	//An item can have multiple values but they must all be a homogenous type, e.g. an array
			throw std::system_error(makeJSBaseError(badValue));
			//Determine if this element acts as an object/array wrapper for values
			//The package will have an outer object wrapper (even if an array) if the outer element has a name that differs from the inner item
		bool isWrapper = (inventory.size() > 1) || (identity.stage == root) ||
				(!identity.name.empty() && !inventory.begin()->identity().name.empty() && (inventory.begin()->identity() != identity));
			//An array package will have a single item within more than one possible value
		bool isArray = !isWrapper && (inventory.size() == 1) && !(inventory.begin()->maximum() == 1),
			 isFirstItem = true;
		if (isWrapper)
			exporter.writeTag(tag, nameSpace, JSBaseIdentity::Type::objectStart, depth++);
		else if (isArray)
			exporter.writeTag(tag, nameSpace, JSBaseIdentity::Type::arrayStart, depth);
		auto sequence = inventory.sequence();
		for (auto& entry : sequence) {
			auto item = *entry.second;
			if (!item.required || (item.available == 0))
				continue;
			if (isFirstItem)
				isFirstItem = false;
			else
				exporter.write(",");
			auto entryNameSpace{item.identity().group.value_or(String())};
				//Each package item may have multiple available cargo items to export
			auto limit = item.available;
			bool isItemArray = item.isRepeating(),
				 isFirstValue = true;
			if (isItemArray)
				exporter.writeTag(item.identity().name, entryNameSpace, JSBaseIdentity::Type::arrayStart, depth);
			for (item.available = 0; item.available < limit; ++item.available) {
				auto content = cargo.getCargo(item);
				if (!content)
					break;	//Discontinue an inventory item when the supply runs out
				if (isFirstValue)
					isFirstValue = false;
				else
					exporter.write(",");
				doJSBaseExport(*content, isItemArray ? item.identity() : JSBaseIdentity{item.identity()}.atStage(object),
							 exporter, (dynamic_cast<Package*>(content.get()) == nullptr) ? depth : depth + ((identity.stage == root) ? 0 : 1));
			}
			if (isItemArray)
				exporter.writeTag(String{}, String{}, JSBaseIdentity::Type::arrayEnd, depth);
		}
		if (isWrapper)
			exporter.writeTag(String{}, String{}, JSBaseIdentity::Type::objectEnd, --depth);
		else if (isArray)
			exporter.writeTag(String{}, String{}, JSBaseIdentity::Type::arrayEnd, depth);
	} //doJSBaseExport
	
}

/*--------------------------------------------------------------------
	Send cargo as XML to a specified destination
 
	cargo: The cargo to be sent as JS::Base
	identity: The cargo identity (name, optional namespace)
	destination: A reference to a JS::Base object (will be populated by this function)
  --------------------------------------------------------------------*/
void JSBaseTransport::send(Cargo&& cargo, const Identity& identity, GS::Ref<JS::Base> destination) const {
	doJSBaseExport(cargo, JSBaseIdentity(identity).atStage(root), destination);
} //JSBaseTransport::send


/*--------------------------------------------------------------------
	Receive cargo from a specified XML source
 
	cargo: The cargo to receive the JS::Base data
	identity: The cargo identity (name, optional namespace)
	source: A reference to a JS::Base object
  --------------------------------------------------------------------*/
void JSBaseTransport::receive(Cargo&& cargo, const Identity& identity, GS::Ref<JS::Base> source) const {
	doJSBaseImport(cargo, JSBaseIdentity(identity).atStage(root), source);
} //JSBaseTransport::receive
