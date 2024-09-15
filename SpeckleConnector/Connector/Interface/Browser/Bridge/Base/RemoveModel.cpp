#include "Connector/Interface/Browser/Bridge/Base/RemoveModel.h"

#include "Connector/Connector.h"
#include "Connector/Database/ModelCardDatabase.h"
#include "Connector/Interface/Browser/Bridge/Base/Arg/DocumentInfo.h"

using namespace active::container;
using namespace active::serialise;
using namespace connector::database;
using namespace connector::record;
using namespace connector::interfac::browser::bridge;
using namespace speckle::utility;

namespace {
	
	using WrappedValue = active::serialise::CargoHold<PackageWrap, DocumentInfo>;

}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
RemoveModel::RemoveModel() : BridgeMethod{"RemoveModel", [&](const ModelCardEventWrapper& card) {
		return run(card.get());
}} {}


/*--------------------------------------------------------------------
	Add a model card to document storage
 
	card: The card to add
  --------------------------------------------------------------------*/
void RemoveModel::run(const ModelCard& card) const {
	if (auto modelCardDBase = connector()->getModelCardDatabase(); modelCardDBase != nullptr)
		modelCardDBase->erase(card.getID());
} //RemoveModel::run
