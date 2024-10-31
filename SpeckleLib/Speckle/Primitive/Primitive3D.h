#ifndef SPECKLE_PRIMITIVE_3D
#define SPECKLE_PRIMITIVE_3D

#include "Speckle/Database/Content/BIMRecord.h"

namespace speckle::primitive {
	
	/*!
	 Interface for all 3D primitives
	 */
	class Primitive3D : public speckle::database::BIMRecord {
	public:
		
		using base = speckle::database::BIMRecord;
		
		/*!
		 Default constructor
		 @param unit The recordc unit type
		 */
		Primitive3D(active::measure::LengthType unit = active::measure::LengthType::metre) : base{unit} {}
		/*!
		 Constructor
		 @param ID The record ID
		 @param tableID The parent table ID
		 @param unit The record unit type
		 */
		Primitive3D(const speckle::utility::Guid& ID, const speckle::utility::Guid& tableID,
					std::optional<active::measure::LengthType> unit = active::measure::LengthType::metre) : base{ID, tableID, unit} {}

		/*!
			Object cloning
			@return A clone of this object
		*/
		Primitive3D* clonePtr() const override = 0;
	};
	
}

#endif	//SPECKLE_PRIMITIVE_3D
