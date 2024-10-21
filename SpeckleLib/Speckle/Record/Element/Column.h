#ifndef SPECKLE_RECORD_ELEMENT_COLUMN
#define SPECKLE_RECORD_ELEMENT_COLUMN

#include "Speckle/Record/Element/ColumnSegment.h"
#include "Speckle/Record/Element/Element.h"
#include "Speckle/Record/Element/Interface/Assembly/Path.h"
#include "Speckle/Record/Element/Interface/SegmentedColumn.h"

namespace speckle::record::element {
	
	class ColumnSegment;
	
	/*!
	 BIM column class
	 */
	class Column : public Element, public SegmentedColumn, public assembly::Path {
	public:

		// MARK: - Types
		
		using base = Element;
			///Unique pointer
		using Unique = std::unique_ptr<Column>;
			///Shared pointer
		using Shared = std::shared_ptr<Column>;
			///Optional
		using Option = std::optional<Column>;

		// MARK: - Constructors
		
		using base::base;
		
		/*!
		 Default constructor
		 */
		Column();
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param elemData Archicad element data
		 @param tableID The column element ID (AC database, e.g. floor plan, 3D)
		 */
		Column(const API_Element& elemData, const speckle::utility::Guid& tableID);
#endif
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		Column(const Column& source);
		/*!
		 Destructor
		 */
		~Column();

		/*!
		 Object cloning
		 @return A clone of this object
		 */
		Column* clonePtr() const override { return new Column{*this}; }


		// MARK: - Functions (const)

		/*!
		 Get the BIM application parent table ID
		 @return The BIM table ID
		 */
		virtual database::BIMRecordID getTableID() const override { return Element::getTableID(); }
		/*!
		 Get the element body
		 @return nullptr (Columns don't explicitly have a 3D body - this comes from its child segments)
		 */
		virtual Body* getBody() const override { return nullptr; }
		/*!
		 Get the number of segments in the path (elements must override according to requirements)
		 @return The segment count
		 */
		virtual size_t getSegmentCount() const override { return SegmentedColumn::getSegmentCount(); }
		/*!
		 Get a segment from the path (elements must override according to requirements)
		 @param index The index of the required segment
		 @return The requested segment (nullptr on failure)
		 */
		virtual ColumnSegment* getSegment(size_t index) const override { return SegmentedColumn::getSegment(index); }
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
		/*!
		 Load the element memo structure (elements must override according to requirements)
		 @param filter Filter bits specifying memo requirements
		 */
		virtual void loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const override;
		
	private:
		class Data;
			///The column data
		std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_COLUMN