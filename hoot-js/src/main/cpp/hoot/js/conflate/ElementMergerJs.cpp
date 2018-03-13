#include "ElementMergerJs.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/js/ScriptMerger.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterion.h>
#include <hoot/core/visitors/ElementIdSetVisitor.h>
#include <hoot/core/util/MapUtils.h>

// Qt
#include <QString>

// std
#include <set>
#include <utility>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementMergerJs)

ElementMergerJs::ElementMergerJs()
{
}

ElementMergerJs::~ElementMergerJs()
{
}

void ElementMergerJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  exports->Set(String::NewFromUtf8(current, "elementMerge"),
               FunctionTemplate::New(current, _jsElementMerge)->GetFunction());
}

void ElementMergerJs::_jsElementMerge(const FunctionCallbackInfo<Value>& args)
{
  LOG_INFO("Merging elements...");

  Isolate* current = args.GetIsolate();
  try
  {
    HandleScope scope(current);
    if (args.Length() != 1)
    {
      args.GetReturnValue().Set(
        current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
          "Expected one argument to 'elementMerge'."))));
      return;
    }

    OsmMapPtr map(node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject())->getMap());
    LOG_VART(map->getElementCount());
    _mergeElements(map, current);
    LOG_VART(map->getElementCount());

    Handle<Object> returnMap = OsmMapJs::create(map);
    args.GetReturnValue().Set(returnMap);
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

QString ElementMergerJs::_mergeTypeToString(const MergeType& mergeType)
{
  switch (mergeType)
  {
    case MergeType::BuildingToBuilding:
      return "BuildingToBuilding";

    case MergeType::PoiToPolygon:
      return "PoiToPolygon";

    case MergeType::PoiToPoi:
      return "PoiToPoi";

    case MergeType::AreaToArea:
      return "AreaToArea";

    default:
      throw HootException("Invalid merge type.");
  }
  return "";
}

void ElementMergerJs::_mergeElements(OsmMapPtr map, Isolate* current)
{  
  const MergeType mergeType = _determineMergeType(map);
  LOG_VART(_mergeTypeToString(mergeType));
  _validateMergeTargetElement(map, mergeType);

  //We're using a mix of generic conflation scripts and custom built classes to merge features
  //here, depending on the feature type.

  bool scriptMerge = false;
  const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
  LOG_VART(mergeTargetId);
  switch (mergeType)
  {
    case MergeType::BuildingToBuilding:
      _mergeBuildings(map, mergeTargetId);
      break;

    case MergeType::PoiToPolygon:
      _mergePoiAndPolygon(map, mergeTargetId);
      break;

    case MergeType::PoiToPoi:
      _mergePois(map, mergeTargetId, current);
      scriptMerge = true;
      break;

    case MergeType::AreaToArea:
      _mergeAreas(map, mergeTargetId, current);
      scriptMerge = true;
      break;

    default:
      throw HootException("Invalid merge type.");
  }
  LOG_VART(scriptMerge);

  ElementPtr mergedElement = map->getElement(mergeTargetId);
  //if (!scriptMerge)
  //{
    //We only need to set the resulting merged element's status to conflated when the ScriptMerger
    //wasn't used, as it does so automatically.
    mergedElement->setStatus(Status(Status::Conflated));
    mergedElement->getTags()[MetadataTags::HootStatus()] = "3";
  //}
  mergedElement->getTags().remove(MetadataTags::HootMergeTarget());
  LOG_VART(mergedElement);
}

ElementId ElementMergerJs::_getMergeTargetFeatureId(ConstOsmMapPtr map)
{
  TagKeyCriterion mergeTagFilter(MetadataTags::HootMergeTarget());
  ElementIdSetVisitor idSetVis;
  FilteredVisitor filteredVis(mergeTagFilter, idSetVis);
  map->visitRo(filteredVis);
  const std::set<ElementId>& mergeTargetIds = idSetVis.getElementSet();
  assert(mergeTargetIds.size() == 1);
  return *mergeTargetIds.begin();
}

void ElementMergerJs::_validateMergeTargetElement(ConstOsmMapPtr map, const MergeType& mergeType)
{
  LOG_VART(_mergeTypeToString(mergeType));
  const long numMergeTargets =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootMergeTarget())),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  LOG_VART(numMergeTargets);
  if (numMergeTargets != 1)
  {
    throw IllegalArgumentException(
      "Input map must have one feature marked with a " + MetadataTags::HootMergeTarget() + " tag.");
  }
  else if (mergeType == MergeType::PoiToPolygon)
  {
    //For POI to polygon conflation we always merge the POI into the polygon, so the poly must
    //be the specified target element.
    const long numPolyMergeTargets =
      (long)FilteredVisitor::getStat(
        ElementCriterionPtr(
          new ChainCriterion(
            ElementCriterionPtr(new PoiPolygonPolyCriterion()),
            ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootMergeTarget())))),
        ConstElementVisitorPtr(new ElementCountVisitor()),
        map);
    LOG_VART(numPolyMergeTargets);
    if (numPolyMergeTargets != 1)
    {
      throw IllegalArgumentException(
        "Input map must have one polygon feature marked with a " + MetadataTags::HootMergeTarget() +
        " tag.");
    }
  }
}

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map)
{
  MergeType mergeType;
  //Making sure maps don't come in mixed, so callers don't have the expectation that they can merge
  //multiple feature types within the same map.  Any features existing in the map outside of what
  //we know how to merge will just pass through.
  const bool containsPolys = MapUtils::containsPolys(map);   //this is the poi/poly definition of poly
  const bool containsAreas = MapUtils::containsAreas(map); //non-building areas
  const bool containsBuildings = MapUtils::containsBuildings(map);
  const bool containsPois = MapUtils::containsPois(map); //general poi definition
  if (MapUtils::containsOnePolygonAndOnePoi(map))
  {
    mergeType = MergeType::PoiToPolygon;
  }
  else if (MapUtils::containsTwoOrMorePois(map) && !containsPolys && !containsAreas &&
           !containsBuildings)
  {
    mergeType = MergeType::PoiToPoi;
  }
  else if (MapUtils::containsTwoOrMoreBuildings(map) && !containsAreas && !containsPois)
  {
    mergeType = MergeType::BuildingToBuilding;
  }
  else if (MapUtils::containsTwoOrMoreAreas(map) && !containsBuildings && !containsPois)
  {
    mergeType = MergeType::AreaToArea;
  }
  else
  {
    throw IllegalArgumentException(
      QString("Invalid inputs to element merger.  Inputs must be one of the following:") +
      QString("1) two or more POIs, 2) two or more buildings, 3) two or more areas, or ") +
      QString("4) one POI and one polygon."));
  }
  return mergeType;
}

void ElementMergerJs::_mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId)
{
  //TODO: BuildingMerger keeps the more complex geometry, I think.  Should we add in the
  //configurableoption to keep the ref geometry?

  LOG_INFO("Merging buildings...");
  LOG_VART(mergeTargetId);

  //TODO: Is it possible to just load up all the pairs and call apply once here?

  int buildingsMerged = 0;

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const ConstWayPtr& way = wayItr->second;
    if (way->getElementId() != mergeTargetId && OsmSchema::getInstance().isBuilding(way))
    {
      LOG_VART(way);
      std::set<std::pair<ElementId, ElementId> > pairs;
      pairs.insert(std::pair<ElementId, ElementId>(mergeTargetId, way->getElementId()));
      BuildingMerger merger(pairs);
      LOG_VART(pairs.size());
      std::vector<std::pair<ElementId, ElementId> > replacedElements;
      merger.apply(map, replacedElements);
      buildingsMerged++;
    }
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator relItr = relations.begin(); relItr != relations.end(); ++relItr)
  {
    const ConstRelationPtr& relation = relItr->second;
    if (relation->getElementId() != mergeTargetId && OsmSchema::getInstance().isBuilding(relation))
    {
      LOG_VART(relation);
      std::set<std::pair<ElementId, ElementId> > pairs;
      pairs.insert(std::pair<ElementId, ElementId>(
        mergeTargetId, relation->getElementId()));
      BuildingMerger merger(pairs);
      LOG_VART(pairs.size());
      std::vector<std::pair<ElementId, ElementId> > replacedElements;
      merger.apply(map, replacedElements);
      buildingsMerged++;
    }
  }

  LOG_INFO("Merged " << buildingsMerged << " buildings.");
}

void ElementMergerJs::_mergePoiAndPolygon(OsmMapPtr map, const ElementId& mergeTargetId)
{
  //Trying to merge more than one POI into the polygon has proven problematic due to the building
  //merging logic.  Merging more than one POI isn't a requirement, so only doing 1:1 merging right
  //now.
  LOG_INFO("Merging one POI and one polygon...");
  LOG_VART(mergeTargetId);

  PoiPolygonPoiCriterion poiFilter;
  ElementIdSetVisitor idSetVis;
  FilteredVisitor filteredVis(poiFilter, idSetVis);
  map->visitRo(filteredVis);
  const std::set<ElementId>& poiIds = idSetVis.getElementSet();
  assert(poiIds.size() == 1);   //we've already validated this input
  const ElementId poiId = *poiIds.begin();
  LOG_VART(poiId);

  std::set<std::pair<ElementId, ElementId> > pairs;
  //Ordering doesn't matter here, since the poi is always merged into the poly.
  pairs.insert(std::pair<ElementId, ElementId>(mergeTargetId, poiId));
  PoiPolygonMerger merger(pairs);
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(map, replacedElements);

  LOG_INFO("Merged POI into the polygon.");
}

void ElementMergerJs::_mergeAreas(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current)
{
  LOG_INFO("Merging areas...");

  // Instantiate script merger
  boost::shared_ptr<PluginContext> script(new PluginContext());
  v8::HandleScope handleScope(current);
  v8::Context::Scope context_scope(script->getContext(current));
  script->loadScript(ConfPath::search("Area.js", "rules"), "plugin");
  v8::Handle<v8::Object> global = script->getContext(current)->Global();
  if (global->Has(String::NewFromUtf8(current, "plugin")) == false)
  {
    throw IllegalArgumentException("Expected the script to have exports.");
  }
  Handle<Value> pluginValue = global->Get(String::NewFromUtf8(current, "plugin"));
  Persistent<Object> plugin(current, Handle<Object>::Cast(pluginValue));
  if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
  {
    throw IllegalArgumentException("Expected plugin to be a valid object.");
  }

  int areasMerged = 0;

  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& way = it->second;
    LOG_VART(way->getId());
    LOG_VART(OsmSchema::getInstance().isNonBuildingArea(way));
    if (way->getElementId() != mergeTargetId && OsmSchema::getInstance().isNonBuildingArea(way))
    {
      LOG_TRACE("Merging way area: " << way << " into " << mergeTargetId);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, ElementId::way(way->getId())));
      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the areas down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedWays;
      merger.apply(map, replacedWays);
      LOG_VART(replacedWays.size());

      areasMerged++;
    }
  }

  const RelationMap relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    const ConstRelationPtr& relation = it->second;
    LOG_VART(relation->getId());
    LOG_VART(OsmSchema::getInstance().isNonBuildingArea(relation));
    if (relation->getElementId() != mergeTargetId &&
        OsmSchema::getInstance().isNonBuildingArea(relation))
    {
      LOG_TRACE("Merging relation area: " << relation << " into " << mergeTargetId);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(
        std::pair<ElementId,ElementId>(mergeTargetId, ElementId::relation(relation->getId())));
      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the areas down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedRelations;
      merger.apply(map, replacedRelations);
      LOG_VART(replacedRelations.size());

      areasMerged++;
    }
  }

  LOG_INFO("Merged " << areasMerged << " areas.");
}

void ElementMergerJs::_mergePois(OsmMapPtr map, const ElementId& mergeTargetId, Isolate* current)
{
  LOG_INFO("Merging POIs...");

  // Instantiate script merger
  boost::shared_ptr<PluginContext> script(new PluginContext());
  v8::HandleScope handleScope(current);
  v8::Context::Scope context_scope(script->getContext(current));
  script->loadScript(ConfPath::search("PoiGeneric.js", "rules"), "plugin");
  v8::Handle<v8::Object> global = script->getContext(current)->Global();
  if (global->Has(String::NewFromUtf8(current, "plugin")) == false)
  {
    throw IllegalArgumentException("Expected the script to have exports.");
  }
  Handle<Value> pluginValue = global->Get(String::NewFromUtf8(current, "plugin"));
  Persistent<Object> plugin(current, Handle<Object>::Cast(pluginValue));
  if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
  {
    throw IllegalArgumentException("Expected plugin to be a valid object.");
  }

  // Got in Map with POIs A, B, C, D, E
  //
  // Make a set of pairs to indicate all are same object:
  //   A->B, A->C, A->D, A->E
  //
  // ...then pass those pairs one at a time through the merger, since it only merges pairs
  int poisMerged = 0;
  const NodeMap nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const ConstNodePtr& node = it->second;
    if (node->getId() != mergeTargetId.getId() && OsmSchema::getInstance().isPoi(*node))
    {
      LOG_VART(node);

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, node->getElementId()));
      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the POIs down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedNodes;
      merger.apply(map, replacedNodes);
      LOG_VART(replacedNodes.size());

      poisMerged++;
    }
  }
  LOG_INFO("Merged " << poisMerged << " POIs.");
}

}
