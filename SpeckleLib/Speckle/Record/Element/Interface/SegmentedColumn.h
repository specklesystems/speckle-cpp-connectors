#ifndef SPECKLE_RECORD_ELEMENT_SEGMENTED_COLUMN
#define SPECKLE_RECORD_ELEMENT_SEGMENTED_COLUMN

#include "Speckle/Database/Identity/BIMRecordID.h"
#include "Speckle/Record/Element/Interface/Part.h"

namespace speckle::record::element {
	
	class ColumnSegment;
	
	/*!
	 Interface for a column type that is made up of consecutive segments
	 
	 Note that the child segments use lazy loading to avoid high overheads when accessing data relevant to the parent column only
	 */
	class SegmentedColumn : public Part {
	public:

		// MARK: - Types
		
		using base = Part;
			///Unique pointer
		using Unique = std::unique_ptr<SegmentedColumn>;
			///Shared pointer
		using Shared = std::shared_ptr<SegmentedColumn>;
			///Optional
		using Option = std::optional<SegmentedColumn>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SegmentedColumn();
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		SegmentedColumn(const SegmentedColumn& source);
		/*!
		 Destructor
		 */
		~SegmentedColumn();

		// MARK: - Functions (const)

		/*!
		 Get the BIM application parent table ID
		 @return The BIM table ID
		 */
		virtual database::BIMRecordID getTableID() const = 0;
		/*!
		 Get the number of segments
		 @return The number of segments (0 on error)
		 */
		size_t getSegmentCount() const;
		/*!
		 Get a column segment
		 @param index The index of the required segment
		 @return The requested segment, nullptr on error
		 */
		ColumnSegment* getSegment(size_t index) const;
		
		// MARK: - Functions (mutating)

	protected:
		/*!
		 Return the bits for the part filter required to load the data necessary to build this object
		 @return The required filter bits */
		filter_bits getPartFilter() const;
		/*!
		 Determine if the element part content has been validated (elements must override according to requirements)
		 @return True if the element part content is valid
		 */
		bool isPartValid() const override;
		/*!
		 Load the element memo structure (elements must override according to requirements)
		 @param filter Filter bits specifying memo requirements
		 */
		void loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const override;

		/*!
		 Send the element part back to a memo structure for storage (elements must override according to requirements)
		 @param memo The memo to carry the data
		 @return True if the data was successfully sent
		 */
		bool send(Memo* memo) const override;
		/*!
		 Receive the element memo data from a memo structure (elements must override according to requirements)
		 @param memo The memo carrying the data
		 @return True if the data was successfully received
		 */
		bool receive(const Memo& memo) const override;
		
	private:
		class Data;
			///The segment data - mutable to support lazy loading
		mutable std::unique_ptr<Data> m_data;
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_SEGMENTED_COLUMN
