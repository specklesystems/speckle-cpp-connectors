#include "Connector/Interface/Browser/Bridge/Test/TriggerEvent.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Interface/Browser/Bridge/BrowserBridge.h"

#include <array>

using namespace active::serialise;
using namespace active::setting;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		okID,
		nameID,
		countID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"isOk"},
		Identity{"name"},
		Identity{"count"},
	};

	/*!
	 Object for testing sending events to JS
	 */
	class TestObject final : public active::serialise::Package {
	public:
		
		// MARK: - Public variables
		
			///Sample parameters for testing only
		bool isOK = true;
		speckle::utility::String name = "foo";
		int32_t count = 42;

		// MARK: - Serialisation
		
		/*!
			Fill an inventory with the package items
			@param inventory The inventory to receive the package items
			@return True if the package has added items to the inventory
		*/
		bool fillInventory(active::serialise::Inventory& inventory) const override {
			using enum Entry::Type;
			inventory.merge(Inventory{
				{
					{ fieldID[okID], okID, element },
					{ fieldID[nameID], nameID, element },
					{ fieldID[countID], countID, element },
				},
			}.withType(&typeid(TestObject)));
			return true;
		}
		/*!
			Get the specified cargo
			@param item The inventory item to retrieve
			@return The requested cargo (nullptr on failure)
		*/
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override {
			if (item.ownerType != &typeid(TestObject))
				return nullptr;
			using namespace active::serialise;
			switch (item.index) {
				case okID:
					return std::make_unique<ValueWrap<bool>>(isOK);
				case nameID:
					return std::make_unique<ValueWrap<String>>(name);
				case countID:
					return std::make_unique<ValueWrap<int32_t>>(count);
				default:
					return nullptr;	//Requested an unknown index
			}
		}
	};
	
}

/*--------------------------------------------------------------------
	Default constructor
 --------------------------------------------------------------------*/
TriggerEvent::TriggerEvent() : BridgeMethod{"TriggerEvent", [&](const TriggerEventWrapper& arg) {
		return run(arg);
}} {}


/*--------------------------------------------------------------------
	Trigger an event based on a specified name
 
	eventName: The event name
  --------------------------------------------------------------------*/
void TriggerEvent::run(const speckle::utility::String& eventName) const {
	if (!hasBridge())
		return;
	if (eventName == "emptyTestEvent")
		getBridge()->sendEvent(eventName);
	else if (eventName == "testEvent")
		getBridge()->sendEvent(eventName, std::make_unique<TestObject>());
} //TriggerEvent::run
