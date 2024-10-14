#ifndef SPECKLE_PRIMITIVE_MESH
#define SPECKLE_PRIMITIVE_MESH

#include "Speckle/Database/Content/BIMRecord.h"
#include "Speckle/Utility/String.h"

#ifdef ARCHICAD
#include "ModelMaterial.hpp"
#endif

namespace speckle::primitive {
	
	/*!
	 Class for a 3D mesh
	 */
	class Mesh : public speckle::database::BIMRecord {
	public:

		// MARK: - Types

		using base = speckle::database::BIMRecord;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 @param unit The mesh unit type
		 */
		Mesh(active::measure::LengthType unit = active::measure::LengthType::metre) : base{utility::Guid{true}, utility::Guid{}, unit} {}
		/*!
		 Constructor
		 @param vertices The mesh vertices
		 @param faces The mesh faces (the number of indices in the face followed by the vertex indices)
		 @param colors The mesh face colours
		 @param unit The mesh unit type
		 */
		Mesh(std::vector<double>&& vertices, std::vector<int>&& faces, std::vector<int>&& colors, const ModelerAPI::Material& material,
				active::measure::LengthType unit = active::measure::LengthType::metre) :
				base{utility::Guid{true}, utility::Guid{}, unit}, m_vertices{std::move(vertices)}, m_faces{std::move(faces)}, m_colors{std::move(colors)}, m_material{material} {}

		// MARK: - Functions (const)
		
		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		speckle::utility::String getSpeckleType() const override { return "Objects.Geometry.Mesh"; }
		
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
		std::vector<double> m_vertices;
		std::vector<int> m_faces;
		std::vector<int> m_colors;
#ifdef ARCHICAD
		ModelerAPI::Material m_material;
#endif
	};
	
}

#endif	//SPECKLE_PRIMITIVE_MESH
