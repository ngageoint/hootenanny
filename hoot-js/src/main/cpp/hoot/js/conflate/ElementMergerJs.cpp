#include "ElementMergerJs.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/js/ScriptMerger.h>
//#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/NonBuildingAreaCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterion.h>
#include <hoot/core/visitors/ElementIdSetVisitor.h>

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
  Isolate* current = args.GetIsolate();
  try
  {
    HandleScope scope(current);
    if (args.Length() != 1)
    {
      args.GetReturnValue().Set(
        current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
          "Expected one arguments to 'elementMerge'."))));
      return;
    }

    OsmMapPtr map(node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject())->getMap());
    const MergeType mergeType = _determineMergeType(map);
    _validateMergeTargetElement(map, mergeType);
    _mergeElements(map, mergeType, current);

    Handle<Object> returnMap = OsmMapJs::create(map);
    args.GetReturnValue().Set(returnMap);
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

void ElementMergerJs::_mergeElements(OsmMapPtr map, const MergeType& mergeType, Isolate* current)
{
  //We're using a mix of generic conflation scripts and custom built classes to merge features
  //here, depending on the feature type.

  ElementId mergedId;
  bool scriptMerge = false;
  const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
  LOG_VART(mergeTargetId);
  switch (mergeType)
  {
    case MergeType::BuildingToBuilding:
      _mergeBuildings(map, mergeTargetId);
      break;

    case MergeType::PoiToPolygon:
      //poi/poly can only merge into the poly, so no need to send in the target id
      _mergePoiAndPolygon(map);
      break;

    case MergeType::PoiToPoi:
      _mergePois(map, current);
      scriptMerge = true;
      break;

    case MergeType::AreaToArea:
      _mergeAreas(map, current);
      scriptMerge = true;
      break;

    default:
      throw HootException("Invalid merge type.");
  }

  LOG_VART(map->getElementCount());
  ElementPtr mergedElement = map->getElement(mergeTargetId);
  LOG_VART(mergedElement);

  //We only need to set the resulting merged element's status to conflated when the ScriptMerger
  //wasn't used, as it does so automatically.
  if (!scriptMerge)
  {
    LOG_VART(mergedId)
    mergedElement->setStatus(Status(Status::Conflated));
    mergedElement->getTags()[MetadataTags::HootStatus()] = "3";
  }

  mergedElement->getTags()[MetadataTags::HootMergeTarget()] = "";
  mergedElement->getTags().removeEmptyTags();

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
  long numMergeTargets;
  if (mergeType == MergeType::PoiToPolygon)
  {
    //For POI to polygon conflation we always merge the POI into the polygon, so the poly must
    //be the specified target element.
    numMergeTargets =
      (long)FilteredVisitor::getStat(
        ElementCriterionPtr(
          new ChainCriterion(
            ElementCriterionPtr(new PoiPolygonPolyCriterion()),
            ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootMergeTarget())))),
        ConstElementVisitorPtr(new ElementCountVisitor()),
        map);
  }
  else
  {
    numMergeTargets =
      (long)FilteredVisitor::getStat(
        ElementCriterionPtr(new TagKeyCriterion(MetadataTags::HootMergeTarget())),
        ConstElementVisitorPtr(new ElementCountVisitor()),
        map);
  }
  if (numMergeTargets != 1)
  {
    throw IllegalArgumentException(
      "Input map must have one feature marked with a " + MetadataTags::HootMergeTarget() +
      " tag.");
  }
}

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map)
{
  MergeType mergeType;
  if (_containsOnlyTwoOrMorePois(map))
  {
    mergeType = MergeType::PoiToPoi;
  }
  else if (_containsTwoBuildings(map))
  {
    mergeType = MergeType::BuildingToBuilding;
  }
  else if (_containsTwoOrMoreAreas(map))
  {
    mergeType = MergeType::AreaToArea;
  }
  else if (_containsOnePolygonAndOneOrMorePois(map))
  {
    mergeType = MergeType::PoiToPolygon;
  }
  else
  {
    throw IllegalArgumentException(
      QString("Invalid inputs to element merger.  Inputs must be one of the following:") +
      QString("1) two or more POIs, 2) exactly two buildings, 3) two or more areas, or ") +
      QString("4) one or more POIs and exactly one polygon."));
  }
  return mergeType;
}

bool ElementMergerJs::_containsOnlyTwoOrMorePois(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  return poiCount >= 2 && map->getWayCount() == 0 && map->getRelationCount() == 0;
}

bool ElementMergerJs::_containsTwoBuildings(ConstOsmMapPtr map)
{
  const long buildingCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new BuildingCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  return buildingCount == 2;
}

bool ElementMergerJs::_containsTwoOrMoreAreas(ConstOsmMapPtr map)
{
  const long areaCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new NonBuildingAreaCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  return areaCount >= 2;
}

bool ElementMergerJs::_containsOnePolygonAndOneOrMorePois(ConstOsmMapPtr map)
{
  const long poiCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPoiCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  const long polyCount =
    (long)FilteredVisitor::getStat(
      ElementCriterionPtr(new PoiPolygonPolyCriterion()),
      ConstElementVisitorPtr(new ElementCountVisitor()),
      map);
  return poiCount >= 1 && polyCount == 1;
}

void ElementMergerJs::_mergeBuildings(OsmMapPtr map, const ElementId& mergeTargetId)
{
  LOG_INFO("Merging two buildings...");

  //This logic will work whether constituent way nodes/relation members are passed in or not.  See
  //additional notes in the method description.

  LOG_VART(mergeTargetId);
  LOG_VART(map->getElementCount());

  int buildingCount = 0;
  ElementId buildingElementId;
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const int wayId = wayItr->first;
    WayPtr way = map->getWay(wayId);
    if (OsmSchema::getInstance().isBuilding(way))
    {
      if (way->getElementId() != mergeTargetId)
      {
        LOG_VART(way);
        buildingElementId = ElementId::way(wayId);
      }
      buildingCount++;
    }
  }
  if (buildingElementId.isNull())
  {
    const RelationMap& relations = map->getRelations();
    for (RelationMap::const_iterator relItr = relations.begin(); relItr != relations.end(); ++relItr)
    {
      const int relationId = relItr->first;
      RelationPtr relation = map->getRelation(relationId);
      if (OsmSchema::getInstance().isBuilding(relation))
      {
        if (relation->getElementId() != mergeTargetId)
        {
          LOG_VART(relation);
          buildingElementId = ElementId::relation(relationId);
        }
      }
      buildingCount++;
    }
  }
  if (buildingCount == 0)
  {
    throw IllegalArgumentException("No buildings passed to building merger.");
  }
  if (buildingCount > 2)
  {
    throw IllegalArgumentException("More than two buildings passed to building merger.");
  }

  LOG_VART(buildingElementId);

  std::set<std::pair<ElementId, ElementId> > pairs;
  pairs.insert(std::pair<ElementId, ElementId>(mergeTargetId, buildingElementId));
  BuildingMerger merger(pairs);
  LOG_VART(pairs.size());
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(map, replacedElements);
}

void ElementMergerJs::_mergePoiAndPolygon(OsmMapPtr map)
{
  LOG_INFO("Merging a POI and a polygon...");

  //This logic will work whether constituent way nodes/relation members are passed in or not.  See
  //additional notes in the method description.

  LOG_VART(map->getElementCount());

  int poiCount = 0;
  ElementId poiElementId;
  //Status poiStatus;
  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator nodeItr = nodes.begin(); nodeItr != nodes.end(); ++nodeItr)
  {
    const int nodeId = nodeItr->first;
    NodePtr node = map->getNode(nodeId);
    if (OsmSchema::getInstance().isPoiPolygonPoi(node))
    {
      //TODO: Arbitrarily setting the status doesn't seem right.  Should be able to assume that
      //that the incoming features always have a status, as is done with AreaMerger.

      //If the POI has no status, arbitrarily make the POI unknown1 and the poly unknown2.  Make
      //sure both input poi and poly have different input statuses.
//      if (node->getStatus() != Status::Unknown1 && node->getStatus() != Status::Unknown2)
//      {
//        node->setStatus(Status::Unknown1);
//      }
      LOG_VART(node);
      //poiStatus = node->getStatus();
      poiElementId = ElementId::node(nodeId);
      poiCount++;
    }
  }
  if (poiCount == 0)
  {
    throw IllegalArgumentException("No POI passed to POI/Polygon merger.");
  }
  if (poiCount > 1)
  {
    throw IllegalArgumentException("More than one POI passed to POI/Polygon merger.");
  }

  int polyCount = 0;
  ElementId polyElementId;
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator wayItr = ways.begin(); wayItr != ways.end(); ++wayItr)
  {
    const int wayId = wayItr->first;
    WayPtr way = map->getWay(wayId);
    if (OsmSchema::getInstance().isPoiPolygonPoly(way))
    {
      //see comment in node loop above
//      if (way->getStatus() != Status::Unknown1 && way->getStatus() != Status::Unknown2)
//      {
//        if (poiStatus == Status::Unknown1)
//        {
//          way->setStatus(Status::Unknown2);
//        }
//        else
//        {
//          way->setStatus(Status::Unknown1);
//        }
//      }
      LOG_VART(way);
      polyElementId = ElementId::way(wayId);
      polyCount++;
    }
  }
  if (polyElementId.isNull())
  {
    const RelationMap& relations = map->getRelations();
    for (RelationMap::const_iterator relItr = relations.begin(); relItr != relations.end(); ++relItr)
    {
      const int relationId = relItr->first;
      RelationPtr relation = map->getRelation(relationId);
      if (OsmSchema::getInstance().isPoiPolygonPoly(relation))
      {
        //see comment in node loop above
//        if (relation->getStatus() != Status::Unknown1 && relation->getStatus() != Status::Unknown2)
//        {
//          if (poiStatus == Status::Unknown1)
//          {
//            relation->setStatus(Status::Unknown2);
//          }
//          else
//          {
//            relation->setStatus(Status::Unknown1);
//          }
//        }
        LOG_VART(relation);
        polyElementId = ElementId::relation(relationId);
        polyCount++;
      }
    }
  }
  if (polyCount == 0)
  {
    throw IllegalArgumentException("No polygon passed to POI/Polygon merger.");
  }
  if (polyCount > 1)
  {
    throw IllegalArgumentException("More than one polygon passed to POI/Polygon merger.");
  }

  LOG_VART(poiElementId);
  LOG_VART(polyElementId);

  std::set<std::pair<ElementId, ElementId> > pairs;
  pairs.insert(std::pair<ElementId, ElementId>(poiElementId, polyElementId));
  PoiPolygonMerger merger(pairs);
  LOG_VART(pairs.size());
  std::vector<std::pair<ElementId, ElementId> > replacedElements;
  merger.apply(map, replacedElements);
}

void ElementMergerJs::_mergeAreas(OsmMapPtr map, Isolate* current)
{
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

  const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
  LOG_TRACE("Merge target ID: " << mergeTargetId);

  for (WayMap::const_iterator it = map->getWays().begin(); it != map->getWays().end(); ++it)
  {
    if (it->second->getId() != mergeTargetId.getId())
    {
      const ConstWayPtr& way = it->second;

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, ElementId::way(way->getId())));

      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the areas down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedWays;
      merger.apply(map, replacedWays);

      if (replacedWays.size() == 1)
      {
        LOG_TRACE(
          "Area merge: replacing way #" << replacedWays[0].first.getId() <<
          " with updated version of way #" << replacedWays[0].second.getId());
        map->replace(
          map->getElement(replacedWays[0].first), map->getElement(replacedWays[0].second));
      }
    }
  }

  for (RelationMap::const_iterator it = map->getRelations().begin();
       it != map->getRelations().end(); ++it)
  {
    if (it->second->getId() != mergeTargetId.getId())
    {
      const ConstRelationPtr& relation = it->second;

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(
        std::pair<ElementId,ElementId>(mergeTargetId, ElementId::relation(relation->getId())));

      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the areas down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedRelations;
      merger.apply(map, replacedRelations);

      if (replacedRelations.size() == 1)
      {
        LOG_TRACE(
          "Area merge: replacing relation #" << replacedRelations[0].first.getId() <<
          " with updated version of relation #" << replacedRelations[0].second.getId());
        map->replace(
          map->getElement(replacedRelations[0].first), map->getElement(replacedRelations[0].second));
      }
    }
  }
}

void ElementMergerJs::_mergePois(OsmMapPtr map, Isolate* current)
{
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

  //We actually never call this with anything other than two POIs, but might as well leave the
  //functionality in for now even though its inconsistent with the other merging workflows.

  // Got in Map with POIs A, B, C, D, E
  //
  // Make a set of pairs to indicate all are same object:
  //   A->B, A->C, A->D, A->E
  //
  // ...then pass those pairs one at a time through the merger, since it only merges pairs
  const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
  LOG_TRACE("Merge target ID: " << mergeTargetId);
  for (NodeMap::const_iterator it = map->getNodes().begin(); it != map->getNodes().end(); ++it)
  {
    if (it->second->getId() != mergeTargetId.getId())
    {
      const ConstNodePtr& n = it->second;

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, ElementId::node(n->getId())));

      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the POIs down to one.
      ScriptMerger merger(script, plugin, matches);
      std::vector< std::pair< ElementId, ElementId > > replacedNodes;
      merger.apply(map, replacedNodes);

      if (replacedNodes.size() == 1)
      {
        LOG_TRACE(
          "POI merge: replacing node #" << replacedNodes[0].first.getId() <<
          " with updated version of node #" << replacedNodes[0].second.getId());
        map->replaceNode(replacedNodes[0].first.getId(), replacedNodes[0].second.getId());
      }
    }
  }
}

}
