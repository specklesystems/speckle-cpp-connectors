#include "Speckle/Record/Element/ModelElement.h"

#include "Active/Serialise/Package/Wrapper/ContainerWrap.h"
#include "Speckle/Environment/Addon.h"
#include "Speckle/Primitive/Mesh/Mesh.h"
#include "Speckle/Record/Element/Memo.h"
#include "Speckle/Record/Property/Wrapper/PropertiedWrapper.h"
#include "Speckle/SpeckleResource.h"
#include "Speckle/Utility/BIMMemory.h"

#ifdef ARCHICAD
#include <exp.h>
#include <AttributeIndex.hpp>
#include <ConvexPolygon.hpp>
#include <Model.hpp>
#include <ModelElement.hpp>
#include <ModelMaterial.hpp>
#include <ModelMeshBody.hpp>
#include <Sight.hpp>
#endif

using namespace active::measure;
using namespace active::serialise;
using namespace speckle::database;
using namespace speckle::environment;
using namespace speckle::record::attribute;
using namespace speckle::record::element;
using namespace speckle::record::element::quants;
using namespace speckle::record::property;
using namespace speckle::utility;

#include <array>
#include <memory>

namespace speckle::record::element {

	class ModelElement::Data {
	public:
		friend class ModelElement;

#ifdef ARCHICAD
		Data() {}
		Data(const Data& source) : m_cache{std::make_unique<ModelElement::Body>(*source.m_cache)} {}
#endif

	private:
		std::unique_ptr<ModelElement::Body> m_cache;
	};

	
	/*!
	 Cache for materials in Archicad
	 
	 NB: This has been implemented because looking up materials through the API is very slow (can easily triple processing time)
	 This can be eliminated in future if this problem is remedied
	 */
	class ModelElement::FinishCache : public std::unordered_map<Guid, Finish> {
	public:
		using base = std::unordered_map<Guid, Finish>;
		using base::base;
		
		FinishCache() : base{} { activeSession = addon()->getSession(); }
		
			///The active session value when the cache was created - enables the content to be reset between sessions
		uint32_t activeSession = 0;
		
		/*!
		 Validate the cache (ensure matches the active session)
		 @return True if the content is valid
		 */
		bool validate() {
			if (activeSession != addon()->getSession()) {
				m_finishCache.reset();
				return false;
			}
			return true;
		}

	};
	
		///Allocation for the ModelElement finishes cache (used because repeated API access is very slow)
	std::unique_ptr<ModelElement::FinishCache> ModelElement::m_finishCache;
	
}

namespace {

	///Serialisation fields
	enum FieldIndex {
		bodyID,
		materialQuantsID,
		propertyID,
	};

	///Serialisation field IDs
	static std::array fieldID = {
		Identity{"displayValue"},
		Identity{"materialQuantities"},
		Identity{"properties"},
	};
	
#ifdef ARCHICAD
	/*!
	 Collect the IDs of individual parts from an assembly (e.g. stair, curtain wall)
	 @param parts A pointer to the assembly parts
	 @param partIDs The assembly part IDs
	 */
	template<typename T>
	void getPartIDs(T* parts, std::set<API_Guid>& partIDs) {
		GSSize nSubElements = BMGetPtrSize(reinterpret_cast<GSPtr>(parts)) / sizeof(T);
		for (Int32 idx = 0; idx < nSubElements; ++idx)
			partIDs.insert(parts[idx].head.guid);
	}
	
	
	/*!
	 Measure material quantities from the composite materials of a specified element
	 @param guid The ID of the element to measure
	 @param elementQuantity Quantities extracted from the target element (out)
	 @param extendedQuantity Optional extended quantities calculated for some element types
	 @param quantityMask Mask to determine which quantities are required (minimise calculation time)
	 */
	void measureQuantities(const API_Guid& guid, API_ElementQuantity& elementQuantity,
								  API_Quantities& extendedQuantity, const API_QuantitiesMask& quantityMask) {
		extendedQuantity.elements = &elementQuantity;
		GS::Array<API_ElemPartQuantity> elementPartQuantities;
		API_QuantityPar quantityParameters{};
		quantityParameters.minOpeningSize = Eps;
		ACAPI_Element_GetQuantities(guid, &quantityParameters, &extendedQuantity, &quantityMask);
	} //measureQuantities
	
	
	/*!
	 Collect the IDs of individual parts from an element (e.g. stair, curtain wall)
	 @param elemId The element to collect part IDs from
	 @param typeID The element type identifier
	 @param memo The element memo data
	 */
	std::set<API_Guid> collectPartIDs(const API_Guid& elemId, API_ElemTypeID typeID, const Memo& memo) {
		std::set<API_Guid> partIDs{};
		partIDs.insert(elemId);
		if (!memo)
			return partIDs;
		switch (typeID) {
			case API_StairID:
				getPartIDs(memo.root()->stairRisers, partIDs);
				getPartIDs(memo.root()->stairTreads, partIDs);
				getPartIDs(memo.root()->stairStructures, partIDs);
				break;
			case API_RailingID:
				getPartIDs(memo.root()->railingSegments, partIDs);
				getPartIDs(memo.root()->railingPatterns, partIDs);
				getPartIDs(memo.root()->railingRails, partIDs);
				getPartIDs(memo.root()->railingHandrails, partIDs);
				getPartIDs(memo.root()->railingToprails, partIDs);
				getPartIDs(memo.root()->railingBalusterSets, partIDs);
				getPartIDs(memo.root()->railingBalusters, partIDs);
				getPartIDs(memo.root()->railingPanels, partIDs);
				getPartIDs(memo.root()->railingInnerPosts, partIDs);

				getPartIDs(memo.root()->railingNodes, partIDs);
				getPartIDs(memo.root()->railingRailConnections, partIDs);
				getPartIDs(memo.root()->railingHandrailConnections, partIDs);
				getPartIDs(memo.root()->railingToprailConnections, partIDs);
				getPartIDs(memo.root()->railingPosts, partIDs);
				getPartIDs(memo.root()->railingRailEnds, partIDs);
				getPartIDs(memo.root()->railingHandrailEnds, partIDs);
				getPartIDs(memo.root()->railingToprailEnds, partIDs);
				break;
			case API_CurtainWallID:
				getPartIDs(memo.root()->cWallSegments, partIDs);
				getPartIDs(memo.root()->cWallFrames, partIDs);
				getPartIDs(memo.root()->cWallPanels, partIDs);
				getPartIDs(memo.root()->cWallJunctions, partIDs);
				getPartIDs(memo.root()->cWallAccessories, partIDs);
				break;
			default:
				break;
		}
		return partIDs;
	}
#endif
	
}

/*--------------------------------------------------------------------
	Default constructor
  --------------------------------------------------------------------*/
ModelElement::ModelElement() {
} //ModelElement::ModelElement


/*--------------------------------------------------------------------
	Constructor
 
	ID: The record ID
	tableID: The parent table ID
	unit: The record unit type
  --------------------------------------------------------------------*/
ModelElement::ModelElement(const Guid& ID, const Guid& tableID, std::optional<LengthType> unit) : Element{ID, tableID, unit} {
} //ModelElement::ModelElement


/*--------------------------------------------------------------------
	Copy constructor

	source: The object to copy
  --------------------------------------------------------------------*/
ModelElement::ModelElement(const ModelElement& source) : base{source}, Classified{source}, Propertied{source} {
	m_data = source.m_data ? std::make_unique<Data>(*m_data) : nullptr;
} //ModelElement::ModelElement


/*--------------------------------------------------------------------
	Destructor
  --------------------------------------------------------------------*/
ModelElement::~ModelElement() {}


/*--------------------------------------------------------------------
	Get mesh finish from the cache
 
	finishID: The finish ID
 
	return: A pointer to the requested material (nullptr on failure)
  --------------------------------------------------------------------*/
Finish* ModelElement::getFinish(const Guid& finishID) {
	if (!m_finishCache || !m_finishCache->validate())
		return nullptr;
	if (auto iter = m_finishCache->find(finishID); iter != m_finishCache->end())
		return &iter->second;
	return nullptr;
} //ModelElement::getFinish


/*--------------------------------------------------------------------
	Add a mesh finish to the cache
 
	finishID: The finish ID
	finish: The mesh finish
  --------------------------------------------------------------------*/
Finish* ModelElement::cacheFinish(const Guid& finishID, const Finish& finish) {
	if (!m_finishCache || !m_finishCache->validate())
		m_finishCache = std::make_unique<ModelElement::FinishCache>();
	return &m_finishCache->insert({finishID, finish}).first->second;
} //ModelElement::cacheFinish


/*--------------------------------------------------------------------
	Reset the Archicad material cache
  --------------------------------------------------------------------*/
void ModelElement::resetCache() {
	m_finishCache.reset();
} //ModelElement::resetCache


/*--------------------------------------------------------------------
	Get material quantities measured from this element
 
	return: An list of material quantities
  --------------------------------------------------------------------*/
MaterialQuantityList ModelElement::getMaterialQuantities() const {
	using enum Composition;
	MaterialQuantityList result;
	switch (getComposition()) {
		case unordered: {
				//Get the material and spatial measurements
			auto material = getMaterial();
			if (!material)
				return result;
			auto spatialMeasure = getSpatialMeasure();
			if (!spatialMeasure)
				return result;
				//Create a material quantity from the quantity takeoff
			result.push_back({material->getBIMID(), spatialMeasure.area, spatialMeasure.volume});
			break;
		}
		case skinned: {
#ifdef ARCHICAD
			API_ElementQuantity elementQuantity{};
			API_Quantities extendedQuantity{};
			GS::Array<API_CompositeQuantity> compositeQuantity{};
			extendedQuantity.composites = &compositeQuantity;
			API_QuantitiesMask quantityMask{};
				//Set the appropriate masks for composite material volume/area quantity takeoffs
			BIMMemory::setMask(&quantityMask.composites.buildMatIndices);
			BIMMemory::setMask(&quantityMask.composites.volumes);
			BIMMemory::setMask(&quantityMask.composites.projectedArea);
			measureQuantities(getHead().guid, elementQuantity, extendedQuantity, quantityMask);
				//Create material quantities from the quantity takeoff (one oer skin in the composite structure)
			for (auto& skinQuant : compositeQuantity)
				result.push_back({Guid{Guid::fromInt(skinQuant.buildMatIndices.GenerateHashValue())}, skinQuant.projectedArea, skinQuant.volumes});
#endif
			break;
		}
		case profiled:
			break;
	}
	return result;
} //ModelElement::getMaterialQuantities


/*--------------------------------------------------------------------
	Get the element body as a list of faces or Meshes

	return: A pointer to the element body
  --------------------------------------------------------------------*/
ModelElement::Body* ModelElement::getBody() const {
#ifdef ARCHICAD
	if (m_data && m_data->m_cache)
		return m_data->m_cache.get();
	void* dummy = nullptr;
	GSErrCode err = ACAPI_Sight_GetCurrentWindowSight(&dummy);
	if (err != NoError) {
		// TODO: should this throw?
	}
	Modeler::SightPtr currentSightPtr((Modeler::Sight*)dummy); // init the shared ptr with the raw pointer
	ModelerAPI::Model acModel;
	Modeler::IAttributeReader* attrReader = ACAPI_Attribute_GetCurrentAttributeSetReader();
	err = EXPGetModel(currentSightPtr, &acModel, attrReader);
	if (err != NoError) {
		// TODO: should this throw?
	}
	auto elementBody = new ModelElement::Body();
	// Map to collect meshes per material name
	std::unordered_map<String, primitive::Mesh> materialMeshMap;

	std::unique_ptr<Memo> memo;
	loadMemo(APIMemoMask_All, memo);

    auto partIDs = collectPartIDs(getHead().guid, getHead().type.typeID, *memo);
	memo.reset();
	Int32 nElements = acModel.GetElementCount();
	for (Int32 iElement = 1; iElement <= nElements; iElement++) {
		ModelerAPI::Element elem{};
		acModel.GetElement(iElement, &elem);
		API_Guid apiGuid{GSGuid2APIGuid(elem.GetElemGuid())};
		if (partIDs.find(apiGuid) == partIDs.end())
			continue;
		Int32 nBodies = elem.GetTessellatedBodyCount();
		ModelerAPI::Material material{};
		for (Int32 bodyIndex = 1; bodyIndex <= nBodies; ++bodyIndex) {
			ModelerAPI::MeshBody body{};
			elem.GetTessellatedBody(bodyIndex, &body);
			Int32 polyCount = body.GetPolygonCount();
			for (Int32 polyIndex = 1; polyIndex <= polyCount; ++polyIndex) {
				ModelerAPI::Polygon polygon{};
				body.GetPolygon(polyIndex, &polygon);
				ModelerAPI::AttributeIndex attrIndex{};
				polygon.GetMaterialIndex(attrIndex);
				Guid finishID{Guid::fromInt(attrIndex.GetIndex())};
				auto faceFinish = ModelElement::getFinish(finishID);
				if (faceFinish == nullptr) {
					ModelerAPI::Material material{};
					polygon.GetMaterial(&material);
					Finish finish{material};
					faceFinish = ModelElement::cacheFinish(finishID, finish);
				}
				auto materialName = faceFinish->getName();
				if (materialMeshMap.find(materialName) == materialMeshMap.end())
					materialMeshMap[materialName] = primitive::Mesh(*faceFinish);
				Int32 convexPolyCount = polygon.GetConvexPolygonCount();
				for (Int32 convPolyIndex = 1; convPolyIndex <= convexPolyCount; ++convPolyIndex) {
					std::vector<double> vertices;
					ModelerAPI::ConvexPolygon convexPolygon{};
					polygon.GetConvexPolygon(convPolyIndex, &convexPolygon);
					Int32 vertexCount = convexPolygon.GetVertexCount();
					for (Int32 vertexIndex = 1; vertexIndex <= vertexCount; ++vertexIndex) {
						ModelerAPI::Vertex vertex{};
						body.GetVertex(convexPolygon.GetVertexIndex(vertexIndex), &vertex);
							// Collect vertices (as doubles for now, but should be changed to Vertex type)
						vertices.push_back(vertex.x);
						vertices.push_back(vertex.y);
						vertices.push_back(vertex.z);
					}
					materialMeshMap[materialName].appendFace(std::move(vertices));
				}
			}
		}
	}
	for (auto& [materialName, mesh] : materialMeshMap)
		elementBody->push_back(std::move(mesh));
	m_data = std::make_unique<Data>();
	m_data->m_cache.reset(elementBody);
	return m_data->m_cache.get();
#endif
} //ModelElement::getBody


/*--------------------------------------------------------------------
	Fill an inventory with the package items

	inventory: The inventory to receive the package items

	return: True if the package has added items to the inventory
  --------------------------------------------------------------------*/
bool ModelElement::fillInventory(Inventory& inventory) const {
	using enum Entry::Type;
	inventory.merge(Inventory{
		{
			{ fieldID[bodyID], bodyID, element },
			{ fieldID[materialQuantsID], materialQuantsID, element },
			{ fieldID[propertyID], propertyID, element },
		},
	}.withType(&typeid(ModelElement)));
	return base::fillInventory(inventory);
} //ModelElement::fillInventory


/*--------------------------------------------------------------------
	Get the specified cargo

	item: The inventory item to retrieve

	return: The requested cargo (nullptr on failure)
  --------------------------------------------------------------------*/
Cargo::Unique ModelElement::getCargo(const Inventory::Item& item) const {
	if (item.ownerType != &typeid(ModelElement))
		return base::getCargo(item);
	using namespace active::serialise;
	switch (item.index) {
		case bodyID:
			if (auto body = getBody(); body != nullptr)
				return Cargo::Unique{new active::serialise::ContainerWrap{*body}};
			return nullptr;
		case materialQuantsID:
			if (auto quants = getMaterialQuantities(); !quants.empty())
				return Cargo::Unique{new CargoHold<ContainerWrap<MaterialQuantityList>, MaterialQuantityList>{quants}};
			return nullptr;
		case propertyID:
			return nullptr;
			//return std::make_unique<PropertiedWrapper>(*this);
		default:
			return nullptr;	//Requested an unknown index
	}
} //ModelElement::getCargo


/*--------------------------------------------------------------------
	Set to the default package content
  --------------------------------------------------------------------*/
void ModelElement::setDefault() {
	base::setDefault();
	m_data.reset();
} //ModelElement::setDefault
