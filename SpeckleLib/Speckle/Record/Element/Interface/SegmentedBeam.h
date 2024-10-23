#ifndef SPECKLE_RECORD_ELEMENT_SEGMENTED_BEAM
#define SPECKLE_RECORD_ELEMENT_SEGMENTED_BEAM

#include "Speckle/Database/Identity/BIMRecordID.h"
#include "Speckle/Record/Element/Interface/Part.h"

namespace speckle::record::element {
	
	class BeamSegment;
	
	/*!
	 Interface for a beam type that is made up of consecutive segments
	 
	 Note that the child segments use lazy loading to avoid high overheads when accessing data relevant to the parent beam only
	 */
	class SegmentedBeam : public Part {
	public:

		// MARK: - Types
		
		using base = Part;
			///Unique pointer
		using Unique = std::unique_ptr<SegmentedBeam>;
			///Shared pointer
		using Shared = std::shared_ptr<SegmentedBeam>;
			///Optional
		using Option = std::optional<SegmentedBeam>;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		SegmentedBeam();
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		SegmentedBeam(const SegmentedBeam& source);
		/*!
		 Destructor
		 */
		~SegmentedBeam();

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
		 Get a beam segment
		 @param index The index of the required segment
		 @return The requested segment, nullptr on error
		 */
		BeamSegment* getSegment(size_t index) const;
		
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

#endif	//SPECKLE_RECORD_ELEMENT_SEGMENTED_BEAM
