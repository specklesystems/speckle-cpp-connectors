#ifndef SPECKLE_RECORD_ELEMENT_TYPE_SETTING
#define SPECKLE_RECORD_ELEMENT_TYPE_SETTING

#include "Active/Setting/Setting.h"

#ifdef ARCHICAD
#include "APIdefs_Elements.h"
#endif

namespace speckle::record::element {

		///Setting identifying an element type
	class TypeSetting : public active::setting::Setting {
	public:
		
		// MARK: - Types
		
		using base = active::setting::Setting;
			///BIM platform-specific typoe identifier
#ifdef ARCHICAD
		using BIMIdentity = API_ElemType;
#endif
			///Unique pointer
		using Unique = std::unique_ptr<TypeSetting>;
			///Shared pointer
		using Shared = std::shared_ptr<TypeSetting>;
			///Optional
		using Option = std::optional<TypeSetting>;
		
		// MARK: - Constants
		
		static const inline active::utility::NameID ID{active::utility::String{"element type"}};
		
		// MARK: - Constructors
		
		/*!
		 Constructor
		 @param identifier A BIM element type identifier
		 */
		TypeSetting(const BIMIdentity& identifier = BIMIdentity{}) : base{ID}, type{identifier}	{}
#ifdef ARCHICAD
		/*!
		 Constructor
		 @param head An Archicad element header
		 */
		TypeSetting(const API_Elem_Head& head) : base{ID}, type{head.type}	{}
#endif

		/*!
		 Clone method
		 @return A clone of this object
		 */
		virtual TypeSetting* clonePtr() const override { return new TypeSetting{*this}; }

		// MARK: - Public variables
		
			///Platform-specific type identifier
		BIMIdentity type;
		
		// MARK: Operators
		
		/*!
		 Equality operator
		 @param ref The value to compare
		 @return True if the values are identical
		 */
		bool operator==(const BIMIdentity& ref) const;
		/*!
		 Equality operator
		 @param ref The value to compare
		 @return True if the values are identical
		 */
		bool operator==(const Setting& ref) const override;
		/*!
		 Less-than operator
		 @param ref The value to compare
		 @return True if this is less than ref
		 */
		bool operator<(const Setting& ref) const override;
		/*!
		 Assignment operator
		 @param source The object to copy
		 @return A reference to this
		 */
		TypeSetting& operator=(const Setting& source) override;
		/*!
		 Conversion operator
		 @return True if the setting is not empty
		 */
		operator bool() const { return !empty(); }

		// MARK: - Functions (const)
		
		/*!
		 True if the setting is empty
		 */
		bool empty() const override;

		// MARK: Functions (mutating)
		
		/*!
		 Clear the setting (remove any values)
		 */
		void clear() override;
		/*!
		 Assign a default to the setting
		 */
		void setDefault() override { clear(); }
	};
	
}

#endif	//SPECKLE_RECORD_ELEMENT_TYPE_SETTING
