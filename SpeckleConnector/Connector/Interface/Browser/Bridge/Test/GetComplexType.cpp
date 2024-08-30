#include "Connector/Interface/Browser/Bridge/Test/GetComplexType.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Serialise/Package/PackageWrap.h"

#include <array>

using namespace active::serialise;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {

		///Serialisation fields
	enum FieldIndex {
		unID,
		countID,
		isTestID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"id"},
		Identity{"count"},
		Identity{"thisIsABoolean"},
	};
	
	/*!
	 Object for testing Javascript communication (with TestBridge binding)
	 */
	class ComplexType final : public active::serialise::Package {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		ComplexType();
		
		// MARK: - Public variables
		
			///Sample parameters for testing only
		speckle::utility::String ID;
		int32_t count = 0;
		bool testBool = false;

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
					{ fieldID[unID], unID, element, !ID.empty() },
					{ fieldID[countID], countID, element, count != 0 },
					{ fieldID[isTestID], isTestID, element, testBool },
				},
			}.withType(&typeid(ComplexType)));
			return true;
		}
		/*!
			Get the specified cargo
			@param item The inventory item to retrieve
			@return The requested cargo (nullptr on failure)
		*/
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override {
			if (item.ownerType != &typeid(ComplexType))
				return nullptr;
			using namespace active::serialise;
			switch (item.index) {
				case unID:
					return std::make_unique<ValueWrap<String>>(ID);
				case countID:
					return std::make_unique<ValueWrap<int32_t>>(count);
				case isTestID:
					return std::make_unique<ValueWrap<bool>>(testBool);
				default:
					return nullptr;	//Requested an unknown index
			}
		}
	};
	
		///Return type for retrieving the current configuration
	using WrappedValue = CargoHold<PackageWrap, ComplexType>;

}

	//Hashing specialisation
template<>
struct std::hash<ComplexType> {
	auto operator()(const ComplexType& obj) const {
		return hash<std::string>()(obj.ID) ^ rotl(hash<int32_t>()(obj.count), 1) ^ rotl(hash<bool>()(obj.testBool), 2);
	}
};


/*--------------------------------------------------------------------
 Default constructor
  --------------------------------------------------------------------*/
ComplexType::ComplexType() {
		//This is the required test values
	count = static_cast<int32_t>(std::hash<ComplexType>()(*this));
	ID = String{count} + " - I am a string";
} //ComplexType::ComplexType


/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
GetComplexType::GetComplexType() : BridgeMethod{"GetComplexType", [&]() {
		return run();
}} {}


/*--------------------------------------------------------------------
	Get the required object type
 
	return: The required object
  --------------------------------------------------------------------*/
std::unique_ptr<Cargo> GetComplexType::run() const {
	ComplexType object;
	return std::make_unique<WrappedValue>(object);
} //GetComplexType::run
