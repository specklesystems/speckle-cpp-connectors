#ifndef SPECKLE_PRIMITIVE_POLYLINE
#define SPECKLE_PRIMITIVE_POLYLINE

#include "Speckle/Primitive/Primitive3D.h"
#include "Speckle/Record/Attribute/Finish.h"

namespace speckle::primitive {
	
	/*!
	 Class for a 3D polyline
	 */
	class Polyline : public Primitive3D {
	public:

		// MARK: - Types

		using base = Primitive3D;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param unit The polyline unit type
		 */
		Polyline(active::measure::LengthType unit = active::measure::LengthType::metre) : base{ utility::Guid{true}, utility::Guid{}, unit } {}
		/*!
		 Constructor
		 @param points The polyline vertices
		 @param unit The polyline unit type
		 */
		Polyline(std::vector<double>&& points, active::measure::LengthType unit = active::measure::LengthType::metre) :
				base{ utility::Guid{true}, utility::Guid{}, unit }, m_points{ std::move(points) } {}
		
		/*!
			Object cloning
			@return A clone of this object
		*/
		virtual Polyline* clonePtr() const override { return new Polyline{*this}; }

		// MARK: - Functions (const)
		
		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		speckle::utility::String getSpeckleType() const override { return "Objects.Geometry.Polyline"; }
		
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
		active::serialise::Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		/*!
		 Use a manager in (de)serialisation processes
		 @param management The management to use
		 */
		void useManagement(active::serialise::Management* management) const override;
		
	private:
		std::vector<double> m_points;
	};
	
}

#endif	//SPECKLE_PRIMITIVE_POLYLINE
