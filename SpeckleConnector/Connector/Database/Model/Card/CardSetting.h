#ifndef CONNECTOR_DATABASE_MODEL_CARD_SETTING
#define CONNECTOR_DATABASE_MODEL_CARD_SETTING

#include "Active/Serialise/Package/Package.h"
#include "Active/Setting/ValueSetting.h"
#include "Active/Setting/Values/StringValue.h"
#include "Active/Utility/Cloner.h"
#include "Speckle/Utility/Guid.h"
#include "Speckle/Utility/String.h"

namespace connector::database {
	
	/*!
	 A setting attached to a model card, capturing rendering/view states of the model at the time of sending, e.g. level of detail (LoD)
	 */
	class CardSetting : public active::serialise::Package, public active::utility::Cloner {
	public:

		// MARK: - Types
		
		using base = active::serialise::Package;

		// MARK: - Constructors
		
		/*!
		 Default constructor
		 */
		CardSetting() {}
		/*!
		 Copy constructor
		 @param source The object to copy
		 */
		CardSetting(const CardSetting& source) :
				m_title{source.m_title}, m_type{source.m_type}, m_value{clone(*source.m_value)}, m_enum{source.m_enum} {}
		/*!
		 Destructor
		 */
		~CardSetting() {}
		/*!
			Record cloning
			@return A clone of this record
		*/
		virtual CardSetting* clonePtr() const override { return new CardSetting(*this); };
		
		// MARK: - Functions (const)
		
		/*!
		 Get the setting ID
		 @return The setting ID
		 */
		const speckle::utility::String& getID() const { return m_ID; }
		/*!
		 Get the setting title
		 @return The setting title
		 */
		const speckle::utility::String& getTitle() const { return m_title; }
		/*!
		 Get the setting type
		 @return The setting type
		 */
		const speckle::utility::String& getType() const { return m_type; }
		/*!
		 Get the setting value
		 @return The setting value
		 */
		const active::setting::Value& getValue() const { return *m_value; }
		/*!
		 Get the options for the value, i.e. possible values it can hold
		 @return Possible setting values
		 */
		std::vector<speckle::utility::String> getEnum() const { return m_enum; }

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
		
	private:
			///A unique ID for the setting
		speckle::utility::String m_ID = {active::utility::Guid{true}.operator active::utility::String()};
			///The model ID
		speckle::utility::String m_title;
			///The project ID
		speckle::utility::String m_type;
			///The setting value
		active::setting::Value::Unique m_value = std::make_unique<active::setting::StringValue>();
			///Possible values for the setting (as strings)
		std::vector<speckle::utility::String> m_enum;
	};

}

#endif	//CONNECTOR_DATABASE_MODEL_CARD_SETTING
