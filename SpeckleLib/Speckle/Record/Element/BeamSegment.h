#ifndef SPECKLE_RECORD_ELEMENT_BEAM_SEGMENT
#define SPECKLE_RECORD_ELEMENT_BEAM_SEGMENT

#include "Speckle/Record/Element/ModelElement.h"
#include "Speckle/Record/Element/Interface/Assembly/Segment.h"

namespace speckle::record::element {
	
	class SegmentedBeam;
	
	/*!
	 BIM beam class
	 */
	class BeamSegment : public ModelElement, public assembly::Segment {
	public:

		// MARK: - Types
		
		using base = ModelElement;
			///Unique pointer
		using Unique = std::unique_ptr<BeamSegment>;
			///Shared pointer
		using Shared = std::shared_ptr<BeamSegment>;
			///Optional
		using Option = std::optional<BeamSegment>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		BeamSegment();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		BeamSegment(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		BeamSegment(const BeamSegment& source);
		/*!
		 Move constructor
		 @param source The object to move
		 */
		BeamSegment(BeamSegment&& source) noexcept;
		/*!
		 Destructor
		 */
		~BeamSegment();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		BeamSegment* clonePtr() const override { return new BeamSegment{*this}; }


		// MARK: - Functions (const)

#ifdef ARCHICAD
		/*!
		 Get the (immutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		const API_Elem_Head& getHead() const override;
#endif
		
		// MARK: - Functions (mutating)

#ifdef ARCHICAD
		/*!
		 Get the (mutable) API element header data
		 @return The element header data (only use this data for low-level operations - for normal code, call getters/setters)
		 */
		API_Elem_Head& getHead() override;
#endif

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
		Cargo::Unique getCargo(const active::serialise::Inventory::Item& item) const override;
		/*!
		 Set to the default package content
		 */
		void setDefault() override;
		
	protected:
		friend class SegmentedBeam;
		
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param segment The segment element data
		 @param tableID The parent table ID
		 @param cutOrigin Cut at the segment origin
		 @param cutEnd Cut at the segment end
		 @param scheme The segment scheme
		 @param profile The segment profile (nullptr = none)
		 */
		BeamSegment(const API_BeamSegmentType& segment, const speckle::utility::Guid& tableID, const API_AssemblySegmentCutData& cutOrigin,
					  const API_AssemblySegmentCutData& cutEnd, const API_AssemblySegmentSchemeData& scheme,
					  const API_AssemblySegmentProfileData* profile = nullptr);
#endif
		
	private:
		class Data;
			///The beam data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_BEAM_SEGMENT
