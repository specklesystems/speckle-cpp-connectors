#ifndef SPECKLE_RECORD_ELEMENT_MEMO
#define SPECKLE_RECORD_ELEMENT_MEMO

#include "Speckle/Record/Element/Element.h"

#include <memory>
#include <optional>

#ifdef ARCHICAD
struct API_ElementMemo;
#endif

namespace speckle::record::element {
	
	/*!
	 Wrapper for Archicad memo data structure
	 
	 The main purpose for this wrapper is memory safely. In addition to maneging ownership of the memo structure, each handle/pointer allocation
	 within the structure must be released to prevent leaks. This wrapper will ensure these calls are made
	 */
	class Memo : public Element {
	public:

		// MARK: - Types
		
			///Unique pointer
		using Unique = std::unique_ptr<Memo>;
			///Shared pointer
		using Shared = std::shared_ptr<Memo>;
			///Optional
		using Option = std::optional<Memo>;

		// MARK: - Constructors
		
#ifdef ARCHICAD
		/*!
		 Default constructor
		 @param memo A memo structure - this object will take ownership of anything passed to the constructor
		 */
		Memo(std::unique_ptr<API_ElementMemo> memo = nullptr);
#endif
		/*!
		 Destructor
		 */
		virtual ~Memo();

		// MARK: - Operators

		/*!
		 Conversion operator
		 @return True if the memo contains data
		 */
		operator bool() const;

		// MARK: - Functions (const)

#ifdef ARCHICAD
		/*!
		 Get the memo root data
		 @return The memo root data (nullptr on failure)
		 */
		API_ElementMemo* root() const { return m_data.get(); }
#endif

		// MARK: - Functions (mutating)
		
	private:
#ifdef ARCHICAD
			//NB: The following is functionally redundant for memos - requirement of base class
		static API_Elem_Head m_dummy;
		virtual const API_Elem_Head& getHead() const { return m_dummy; }
		virtual API_Elem_Head& getHead() { return m_dummy; }
		
			///The memo data
		std::unique_ptr<API_ElementMemo> m_data;
#endif
	};

}

#endif	//SPECKLE_RECORD_ELEMENT_MEMO
