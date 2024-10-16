#ifndef SPECKLE_RECORD_ELEMENT_COLUMN_SEGMENT
#define SPECKLE_RECORD_ELEMENT_COLUMN_SEGMENT

#include "Speckle/Record/Element/Element.h"
#include "Speckle/Record/Element/Interface/Assembly/Segment.h"

namespace speckle::record::element {
	
	class SegmentedColumn;
	
	/*!
	 BIM column class
	 */
	class ColumnSegment : public Element, public assembly::Segment {
	public:

		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<ColumnSegment>;
			///Shared pointer
		using Shared = std::shared_ptr<ColumnSegment>;
			///Optional
		using Option = std::optional<ColumnSegment>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		ColumnSegment();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The element table ID (AC database, e.g. floor plan, 3D)
		 */
		ColumnSegment(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		ColumnSegment(const ColumnSegment& source);
		/*!
		 Move constructor
		 @param source The object to move
		 */
		ColumnSegment(ColumnSegment&& source);
		/*!
		 Destructor
		 */
		~ColumnSegment();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		ColumnSegment* clonePtr() const override { return new ColumnSegment{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the speckle type identifier
		 @return The speckle type (relevant objects should override as required)
		 */
		speckle::utility::String getSpeckleType() const override { return "Objects.BuiltElements.Element:Objects.BuiltElements.ColumnSegment"; }
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
		friend class SegmentedColumn;
		
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
		ColumnSegment(const API_ColumnSegmentType& segment, const speckle::utility::Guid& tableID, const API_AssemblySegmentCutData& cutOrigin,
					  const API_AssemblySegmentCutData& cutEnd, const API_AssemblySegmentSchemeData& scheme,
					  const API_AssemblySegmentProfileData* profile = nullptr);
#endif
		
	private:
		class Data;
			///The column data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_COLUMN_SEGMENT
