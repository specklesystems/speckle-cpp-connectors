#include "Speckle/Record/Element/Element.h"

#include "Active/Serialise/CargoHold.h"
#include "Active/Serialise/Management/Management.h"
#include "Speckle/Database/BIMElementDatabase.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Environment/Project.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/Record/Element/Setting/TypeSetting.h"
#include "Speckle/Serialise/Collection/ConversionReporter.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/UserCancel.h"

using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::serialise;
using namespace speckle::utility;

#include <array>
#include <memory>
#include <string>

namespace {
    ///Serialisation fields
    enum FieldIndex {
        applicationTypeD,
    };

    ///Serialisation field IDs
    static std::array fieldID = {
        Identity{"applicationType"},
    };

#ifdef ARCHICAD
    String GetElemTypeName(API_ElemTypeID elemTypeID) {
        switch (elemTypeID) {
        case API_WallID: return "Wall";
        case API_ColumnID: return "Column";
        case API_BeamID: return "Beam";
        case API_WindowID: return "Window";
        case API_DoorID: return "Door";
        case API_ObjectID: return "Object";
        case API_LampID: return "Lamp";
        case API_SlabID: return "Slab";
        case API_RoofID: return "Roof";
        case API_MeshID: return "Mesh";
        case API_DimensionID: return "Dimension";
        case API_RadialDimensionID: return "RadialDimension";
        case API_LevelDimensionID: return "LevelDimension";
        case API_AngleDimensionID: return "AngleDimension";
        case API_TextID: return "Text";
        case API_LabelID: return "Label";
        case API_ZoneID: return "Zone";
        case API_HatchID: return "Hatch";
        case API_LineID: return "Line";
        case API_PolyLineID: return "PolyLine";
        case API_ArcID: return "Arc";
        case API_CircleID: return "Circle";
        case API_SplineID: return "Spline";
        case API_HotspotID: return "Hotspot";
        case API_CutPlaneID: return "CutPlane";
        case API_CameraID: return "Camera";
        case API_CamSetID: return "CamSet";
        case API_GroupID: return "Group";
        case API_SectElemID: return "SectElem";
        case API_DrawingID: return "Drawing";
        case API_PictureID: return "Picture";
        case API_DetailID: return "Detail";
        case API_ElevationID: return "Elevation";
        case API_InteriorElevationID: return "InteriorElevation";
        case API_WorksheetID: return "Worksheet";
        case API_HotlinkID: return "Hotlink";
        case API_CurtainWallID: return "CurtainWall";
        case API_CurtainWallSegmentID: return "CurtainWallSegment";
        case API_CurtainWallFrameID: return "CurtainWallFrame";
        case API_CurtainWallPanelID: return "CurtainWallPanel";
        case API_CurtainWallJunctionID: return "CurtainWallJunction";
        case API_CurtainWallAccessoryID: return "CurtainWallAccessory";
        case API_ShellID: return "Shell";
        case API_SkylightID: return "Skylight";
        case API_MorphID: return "Morph";
        case API_ChangeMarkerID: return "ChangeMarker";
        case API_StairID: return "Stair";
        case API_RiserID: return "Riser";
        case API_TreadID: return "Tread";
        case API_StairStructureID: return "StairStructure";
        case API_RailingID: return "Railing";
        case API_RailingToprailID: return "RailingToprail";
        case API_RailingHandrailID: return "RailingHandrail";
        case API_RailingRailID: return "RailingRail";
        case API_RailingPostID: return "RailingPost";
        case API_RailingInnerPostID: return "RailingInnerPost";
        case API_RailingBalusterID: return "RailingBaluster";
        case API_RailingPanelID: return "RailingPanel";
        case API_RailingSegmentID: return "RailingSegment";
        case API_RailingNodeID: return "RailingNode";
        case API_RailingBalusterSetID: return "RailingBalusterSet";
        case API_RailingPatternID: return "RailingPattern";
        case API_RailingToprailEndID: return "RailingToprailEnd";
        case API_RailingHandrailEndID: return "RailingHandrailEnd";
        case API_RailingRailEndID: return "RailingRailEnd";
        case API_RailingToprailConnectionID: return "RailingToprailConnection";
        case API_RailingHandrailConnectionID: return "RailingHandrailConnection";
        case API_RailingRailConnectionID: return "RailingRailConnection";
        case API_RailingEndFinishID: return "RailingEndFinish";
        case API_BeamSegmentID: return "BeamSegment";
        case API_ColumnSegmentID: return "ColumnSegment";
        case API_OpeningID: return "Opening";
#ifdef ServerMainVers_2600
        case API_ExternalElemID: return "ExternalElem";
#endif
        default: return "UnknownElemType";
        }
    }
#endif
}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
Element::Element() {
} //Element::Element


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
Element::Element(const Guid& ID, const Guid& tableID, std::optional<active::measure::LengthType> unit) : base{ID, tableID, unit} {
} //Element::Element


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
Element::Element(const Element& source) : base{ source } {
} //Element::Element


/*--------------------------------------------------------------------
	Move constructor

	source: The object to move
  --------------------------------------------------------------------*/
Element::Element(Element&& source) : base{source} {
} //Element::Element


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
Element::~Element() {}


/*--------------------------------------------------------------------
	Get the element layer
 
	return: The element layer (nullopt if not applicable to the element)
  --------------------------------------------------------------------*/
Layer::Option Element::getLayer() const {
#ifdef ARCHICAD
#ifdef ServerMainVers_2700
	return Layer{ Guid::fromInt(getHead().layer.GenerateHashValue()) };
#else
	return Layer{ Guid::fromInt(getHead().layer) };
#endif
#endif
} //Element::getLayer


/*--------------------------------------------------------------------
	Get the element storey

	return: The element storey (nullopt if the element isn't linked to a storey)
  --------------------------------------------------------------------*/
Storey::Option Element::getStorey() const {
#ifdef ARCHICAD
	return Storey{ getHead().floorInd };
#endif
} //Element::getStorey


/*--------------------------------------------------------------------
	Get a link to the BIM record
 
	return: The BIM record link
  --------------------------------------------------------------------*/
BIMLink Element::getBIMLink() const {
	auto result = base::getBIMLink();
#ifdef ARCHICAD
	result.emplace_back(std::make_unique<TypeSetting>(getHead()));
#endif
	return result;
} //Element::getLink


/*--------------------------------------------------------------------
	Get the localised element type name, e.g. "Wall", "Roof" etc

	return: The type name
  --------------------------------------------------------------------*/
String Element::getLocalisedTypeName() const {
#ifdef ARCHICAD
	GS::UniString typeName;
#ifdef ServerMainVers_2700
	if (auto err = ACAPI_Element_GetElemTypeName(getHead().type, typeName); err != NoError)
#elif defined(ServerMainVers_2600)
	if (auto err = ACAPI_Goodies_GetElemTypeName(getHead().type, typeName); err != NoError)
#else
	if (auto err = ACAPI_Goodies(APIAny_GetElemTypeNameID, (void*) getHead().typeID, &typeName); err != NoError)
#endif
		return addon()->getLocalString(titleStringLib, unknownElementTypeID);
	return typeName;
#endif //ARCHICAD
} //Element::getLocalisedTypeName


/*--------------------------------------------------------------------
	Get the element type name, e.g. "Wall", "Roof" etc

	return: The type name
  --------------------------------------------------------------------*/
#include "Speckle/Environment/Platform.h"
String Element::getTypeName() const {
#ifdef ServerMainVers_2600
    return GetElemTypeName(getHead().type.typeID);
#else
	return GetElemTypeName(getHead().typeID);
#endif
} //Element::getTypeName


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool Element::fillInventory(Inventory& inventory) const {
    using enum Entry::Type;
    inventory.merge(Inventory{
        {
            { fieldID[applicationTypeD], applicationTypeD, element },
        },
        }.withType(&typeid(Element)));
	return base::fillInventory(inventory);
} //Element::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique Element::getCargo(const Inventory::Item& item) const {
    if (item.ownerType != &typeid(Element))
        return base::getCargo(item);
    using namespace active::serialise;
    switch (item.index) {
        case applicationTypeD:
            return Cargo::Unique{ new CargoHold<ValueWrap<String>, String>{ getTypeName() } };
        default:
            return nullptr;	//Requested an unknown index
    }
} //Element::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void Element::setDefault() {
	base::setDefault();
} //Element::setDefault


/*--------------------------------------------------------------------
	Use a manager in (de)serialisation processes
 
	management: The management to use
  --------------------------------------------------------------------*/
void Element::useManagement(Management* management) const {
	if (management != nullptr) {
			//If a conversion report is collected, add this record to the report (also updates progress display in the UI)
		if (auto reporter = management->get<ConversionReporter>(); reporter != nullptr) {
			if (!reporter->logRecord(getBIMID(), {ConversionReporter::Data::Status::success, getTypeName(), getSpeckleType()}))
				throw UserCancel{reporter->getModelCardID()};
		}
	}
} //Element::useManagement


/*--------------------------------------------------------------------
	Load the element memo structure (elements must override according to requirements)
 
	filter: Filter bits specifying memo requirements
  --------------------------------------------------------------------*/
void Element::loadMemo(Part::filter_bits filter, std::unique_ptr<Memo>& memo) const {
		//If the memo data isn't loaded, fetch it now
	if (!memo) {
		auto project = addon()->getActiveProject().lock();
		if (!project)
			return;
		if (auto loaded = project->getElementDatabase()->getMemo(getBIMID(), filter); loaded)
			memo.reset(loaded.release());
	}
} //Element::loadMemo
