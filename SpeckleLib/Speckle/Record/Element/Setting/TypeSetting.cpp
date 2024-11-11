#include "Speckle/Record/Element/Setting/TypeSetting.h"

using namespace speckle::record::element;

/*--------------------------------------------------------------------
	Equality operator
 
	ref: The value to compare
 
	return: True if the values are identical
  --------------------------------------------------------------------*/
bool TypeSetting::operator==(const BIMIdentity& ref) const {
	return type == ref;
} //TypeSetting::operator==


/*--------------------------------------------------------------------
	Equality operator
 
	ref: The value to compare
 
	return: True if the values are identical
  --------------------------------------------------------------------*/
bool TypeSetting::operator==(const Setting& ref) const {
	if (auto refType = dynamic_cast<const TypeSetting*>(&ref); refType != nullptr)
		return type == refType->type;
	return false;
} //TypeSetting::operator==


/*--------------------------------------------------------------------
	Less-than operator
 
	@param ref The value to compare
	@return True if this is less than ref
  --------------------------------------------------------------------*/
bool TypeSetting::operator<(const Setting& ref) const {
	auto refType = dynamic_cast<const TypeSetting*>(&ref);
	if (refType == nullptr)
		return true;
#ifdef ARCHICAD
	if (type.typeID < refType->type.typeID)
		return true;
	if (type.typeID == refType->type.typeID) {
		if (type.variationID < refType->type.variationID)
			return true;
		if (type.variationID == refType->type.variationID)
			return (type.classID < refType->type.classID);
	}
	return false;
#endif
} //TypeSetting::operator==


/*--------------------------------------------------------------------
	Assignment operator
	@param source The object to copy
	@return A reference to this
  --------------------------------------------------------------------*/
TypeSetting& TypeSetting::operator=(const Setting& source) {
	if (auto refType = dynamic_cast<const TypeSetting*>(&source); refType != nullptr)
		type = refType->type;
	return *this;
} //TypeSetting::operator==


/*--------------------------------------------------------------------
	///True if the setting is empty
  --------------------------------------------------------------------*/
bool TypeSetting::empty() const {
#ifdef ARCHICAD
	return type.typeID == API_ZombieElemID;
#endif
} //TypeSetting::operator==


/*--------------------------------------------------------------------
	Clear the setting (remove any values)
  --------------------------------------------------------------------*/
void TypeSetting::clear() {
	type = BIMIdentity{};
} //TypeSetting::clear
