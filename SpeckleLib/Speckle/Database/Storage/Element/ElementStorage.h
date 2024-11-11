#ifndef SPECKLE_DATABASE_ELEMENT_STORAGE
#define SPECKLE_DATABASE_ELEMENT_STORAGE

#include "Speckle/Record/Element/Element.h"

namespace speckle::database {
	
	/*!
	 Fundamental concepts for element storage
	 */
	class ElementStorage {
	public:
		
		// MARK: - Types
		
			///Element storage table types
		enum class TableType {
			primary2D,
			primary3D,
			section,
			elevation,
		};
			///Element filter
		using Filter = std::function<bool(const record::element::Element&)>;
	};
	
}

#endif	//SPECKLE_DATABASE_ELEMENT_STORAGE
