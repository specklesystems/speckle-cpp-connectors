#include "Speckle/Utility/Guid.h"

#ifdef ARCHICAD
/*--------------------------------------------------------------------
	Constructor
 
	guid: An Archicad guid
  --------------------------------------------------------------------*/
speckle::utility::Guid::Guid(const GS::Guid& guid) {
	char buffer[256];
	guid.ConvertToString(buffer);
	*this = active::utility::Guid{active::utility::String{buffer}};
} //Guid::Guid


/*--------------------------------------------------------------------
	Conversion operator
 
	return: An equivalent Archicad guid
  --------------------------------------------------------------------*/
speckle::utility::Guid::operator GS::Guid() const {
	return GS::Guid{operator::active::utility::String().data()};
} //Guid::operator GS::Guid


/*--------------------------------------------------------------------
	Conversion operator
 
	return: An equivalent Archicad guid
  --------------------------------------------------------------------*/
speckle::utility::Guid::operator API_Guid() const {
	return GSGuid2APIGuid(operator::GS::Guid());
} //Guid::operator API_Guid
#endif


