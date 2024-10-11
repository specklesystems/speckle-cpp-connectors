#include "Speckle/Serialise/Detached/DetachedReference.h"

#include "Active/Serialise/Item/Wrapper/ValueWrap.h"
#include "Speckle/Serialise/Detached/DetachmentManager.h"
#include "Speckle/Utility/String.h"

using namespace active::serialise;
using namespace speckle::serialise;
using namespace speckle::utility;

#include <array>

namespace {
	
		///Serialisation fields
	enum FieldIndex {
		refdID,
		speckTypeID,
	};

		///Serialisation field IDs
	static std::array fieldID = {
		Identity{"referencedId"},
		Identity{"speckle_type"},
	};
	
		///Type signature for a reference to a detached object
	String referenceType{"reference"};

}

/*--------------------------------------------------------------------
	Fill an inventory with the package items
 
	inventory: The inventory to receive the package items
 
	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool DetachedReference::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[refdID], refdID, element },
			{ fieldID[speckTypeID], speckTypeID, element },
		},
	}.withType(&typeid(DetachedReference)));
	return true;
} //DetachedReference::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo
 
	item: The inventory item to retrieve
 
	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique DetachedReference::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(DetachedReference))
		return base::get().getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case refdID: {
				//If we don't have an allocated string for receiving a reference (from 'setDefault') then we need to create one
			if (!m_reference) {
					//Ask a manager to send the detached data and provide a reference
				auto detachmentManager = (management() == nullptr) ? nullptr : management()->get<DetachmentManager>();
				if (detachmentManager == nullptr)
					return nullptr;	//TODO: Discuss if this is a serious error - possibly throwing an exception is warranted
				m_reference = detachmentManager->send(std::forward<Package&&>(base::get()), item.identity());
				if (!m_reference)
					throw; //TODO: Throw a more descriptive exception
			}
			std::make_unique<StringWrap>(*m_reference);
		}
		case speckTypeID:
			return std::make_unique<StringWrap>(m_type);
		default:
			return nullptr;	//Requested an unknown index
	}
} //DetachedReference::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void DetachedReference::setDefault() {
	m_type.clear();
	m_reference = String{};
} //DetachedReference::setDefault
