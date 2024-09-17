#ifndef CONNECTOR_MODEL_SEND_FILTER_MOVER
#define CONNECTOR_MODEL_SEND_FILTER_MOVER

#include "Active/Serialise/Package/Wrapper/Mover.h"

namespace connector::record {
	
	/*!
	 Wrapper to box/unbox objects during (de)serialisation, including reading/writing a specified attribute to determine object type
	 
	 Note that a derived class could also define the package handler, allowing the wrapper to be created via a default constructor that is
	 automatically bound to a set of internally defined object types
	 */
	class FilterMover : public active::serialise::Mover {
	public:

		// MARK: - Constructors
		
		/*!
		 Default constructor (primarily for a list of polymorphic objects)
		 */
		FilterMover();
		/*!
		 Constructor (when it will be used for serialisation only on an existing object)
		 @param package A reference to the member variable
		 */
		FilterMover(const Package& package);
		/*!
		 Constructor (for serialisation or deserialisation of a polymorphic member variable held in a unique pointer)
		 @param package A reference to the member variable
		 */
		FilterMover(const Package::Unique& package);
		/*!
		 Constructor (when it will be used for serialisation only on an existing object)
		 @param package A reference to the member variable
		 */
		FilterMover(active::serialise::PackageUniqueWrap&& package);
		
	private:
		/*!
		 Ensure the handler is populated
		 */
		static void validateHandler();
		
			///The handler for model card packages
		static std::shared_ptr<active::serialise::Handler> m_handler;
	};

}
	
#endif //CONNECTOR_MODEL_SEND_FILTER_MOVER
