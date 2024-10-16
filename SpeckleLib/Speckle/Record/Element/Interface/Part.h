#ifndef SPECKLE_RECORD_ELEMENT_PART
#define SPECKLE_RECORD_ELEMENT_PART

#include <memory>
#include <optional>

namespace speckle::record::element {
	
	class Memo;
	
	/*!
	 Interface for an element part, i.e. some component in an assembly element
	 
	 Although this concept is slanted toward Archicad's memo structure, the fundamentals are applicable to any data structures that separate out
	 the constituent parts of an assembly to support lazy loading
	 Note that this class is intended to be an interface - management and/or storage of the data should be provided by subclassing
	 */
	class Part {
	public:
			///Filter bits for memo loading
		using filter_bits = uint64_t;
		
		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<Part>;
			///Shared pointer
		using Shared = std::shared_ptr<Part>;
			///Optional
		using Option = std::optional<Part>;

		// MARK: - Constructors
		
		/*!
		 Destructor
		 */
		virtual ~Part() {}

		// MARK: - Functions (const)

		
		// MARK: - Functions (mutating)

	protected:
		/*!
		 Determine if the element memo data has been successfully loaded and validated
		 @return True if the element memo data is loaded
		 */
		bool isMemoLoaded() const { return m_isLoaded && isPartValid(); }
		/*!
		 Determine if the element memo content has been validated (elements must override according to requirements)
		 @return True if the element memo content is valid
		 */
		virtual bool isPartValid() const = 0;
		/*!
		 Confirm that the element part data is loaded and valid (elements must override according to requirements)
		 @param filter Filter bits specifying memo requirements
		 @return True if the part data is ready to use
		 */
		virtual bool confirmPart(filter_bits filter) const;
		/*!
		 Load the element memo structure (elements must override according to requirements)
		 @param filter Filter bits specifying memo requirements
		 */
		virtual void loadMemo(filter_bits filter, std::unique_ptr<Memo>& memo) const = 0;
		/*!
		 Set whether the element memo data has been loaded (does not establish validity, but prevents multiple attempts to reload)
		 @param state True if the element memo data is loaded
		 */
		void setMemoLoaded(bool state) const { m_isLoaded = state; }

		/*!
		 Send the element part back to a memo structure for storage (elements must override according to requirements)
		 @param memo The memo to carry the data
		 @return True if the data was successfully sent
		 */
		virtual bool send(Memo* memo) const = 0;
		/*!
		 Receive the element memo data from a memo structure (elements must override according to requirements)
		 @param memo The memo carrying the data
		 @return True if the data was successfully received
		 */
		virtual bool receive(const Memo& memo) const = 0;
		
	private:
			///True if the memo data has been loaded into the element (mutable to support lazy loading)
		mutable bool m_isLoaded = false;
	};
	
}

#endif	//SPECKLE_RECORD_ELEMENT_PART
