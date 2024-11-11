#ifndef SPECKLE_UTILITY_USER_CANCEL
#define SPECKLE_UTILITY_USER_CANCEL

#include "Speckle/Database/Identity/RecordID.h"
#include "Speckle/Utility/Exception.h"

namespace speckle::utility {
	
	/*!
	 Exception thrown to signal the user has cancelled a process
	*/
	class UserCancel : public Exception {
	public:
		/*!
		 Default constructor
		 */
		UserCancel() : Exception{{}} {}
		/*!
		 Constructor
		 @param modelCardID The model card ID associated with the cancelled process
		 */
		UserCancel(const database::RecordID& modelCardID) : Exception{{}}, m_modelCardID{modelCardID} {}
		
		/*!
		 Get the ID of the model card associated with the cancelled process
		 @return The model card ID (nullopt = process not associated with a model card)
		 */
		const database::RecordID::Option getModelCardID() const { return m_modelCardID; }
		
	private:
			///The model card ID associated with the cancelled process (nullopt = process not associated with a model card)
		database::RecordID::Option m_modelCardID;
	};
	
}

#endif	//SPECKLE_UTILITY_USER_CANCEL
