#include "Speckle/Serialise/Detached/DetachedWrap.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Active/Utility/BufferOut.h"
#include "Active/Utility/SHA256.h"
#include "Speckle/Serialise/Detached/DetachmentManager.h"
#include "Speckle/Utility/String.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::serialise;
using namespace speckle::utility;

#include <array>

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		refdID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"id"},
	};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool DetachedWrap::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	if (!base::get().fillInventory(inventory))
		return false;
	inventory.merge(Inventory{
		{
			{ fieldID[refdID], refdID, element },
		},
	}.withType(&typeid(DetachedWrap)));
	return true;
} //DetachedWrap::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique DetachedWrap::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(DetachedWrap))
		return base::get().getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case refdID: {
			if (!m_reference) {
				if (m_bufferOut == nullptr)
					return nullptr;	//TODO: Consider throwing an exception here
				auto serialisedData = m_bufferOut->getOutput();
					//Produce a hash from the serialised object. NB: We are currently using only the first 32 chars of the SHA256 hash
				m_reference = (active::utility::SHA256() << serialisedData << String{"}"}).base64Hash().substr(0, 32);
			}
			return std::make_unique<ValueWrap<RecordID>>(*m_reference);
		}
		default:
			return nullptr;	//Requested an unknown index
	}
} //DetachedWrap::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void DetachedWrap::setDefault() {
	m_reference = String{};
} //DetachedWrap::setDefault


/*--------------------------------------------------------------------
	Validate the cargo data
 
	return: True if the data has been validated
  --------------------------------------------------------------------*/
bool DetachedWrap::validate() {
	return m_reference && base::get().validate();
} //DetachedWrap::validate
