#ifndef SPECKLE_INTERFACE_JS_BRIDGE_FUNCTION
#define SPECKLE_INTERFACE_JS_BRIDGE_FUNCTION

namespace speckle::interfac::browser::bridge {
	
	class BrowserBridge;
	
	/*!
	 Interface for any function operating within a browser bridge
	 
	 As a child of a bridge, it may need to facilitate operations through the browser portal - this interface allows the the parent
	 browser bridge to provide access.
	*/
	class BridgeChild {
	public:
		
		// MARK: - Functions (mutating)
		
		/*!
		 Set the parent bridge
		 @param bridge The parent bridge
		 */
		void setBridge(BrowserBridge* bridge) { m_bridge = bridge; }
		
	protected:
		/*!
		 Set the parent bridge
		 @return The parent bridge
		 */
		bool hasBridge() const { return m_bridge != nullptr; }
		/*!
		 Set the parent bridge
		 @return The parent bridge
		 */
		BrowserBridge* getBridge() const { return m_bridge; }
		
	private:
			///The parent browser bridge
		BrowserBridge* m_bridge = nullptr;
	};
	
}

#endif	//SPECKLE_INTERFACE_JS_BRIDGE_FUNCTION
