#include "ElementMergerJs.h"

// Hoot
//#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/js/ScriptMerger.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>

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

    QString scriptPath;
    const MergeType mergeType = _determineMergeType(map, scriptPath);

    if (!scriptPath.isEmpty)
    {
      // Instantiate script merger
      boost::shared_ptr<PluginContext> script(new PluginContext());
      v8::HandleScope handleScope(current);
      v8::Context::Scope context_scope(script->getContext(current));
      script->loadScript(scriptPath, "plugin");
      v8::Handle<v8::Object> global = script->getContext(current)->Global();
      if (global->Has(String::NewFromUtf8(current, "plugin")) == false)
      {
        args.GetReturnValue().Set(
          current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
            "Expected the script to have exports."))));
        return;
      }
      Handle<Value> pluginValue = global->Get(String::NewFromUtf8(current, "plugin"));
      Persistent<Object> plugin(current, Handle<Object>::Cast(pluginValue));
      if (plugin.IsEmpty() || ToLocal(&plugin)->IsObject() == false)
      {
        args.GetReturnValue().Set(
          current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
            "Expected plugin to be a valid object."))));
        return;
      }
    }

    OsmMapPtr mergedMap = _mergeElements(map, plugin);

    Handle<Object> returnMap = OsmMapJs::create(mergedMap);
    args.GetReturnValue().Set(returnMap);
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

OsmMapPtr ElementMergerJs::_mergeElements(OsmMapPtr map, Persistent<Object> plugin)
{
  OsmMapPtr mergedMap(map);

  ElementId mergedId;
  bool scriptMerge = false;
  switch (mergeType)
  {
    case MergeType::PoiToPoi:
      mergedMap = _mergePois(map, plugin);
      scriptMerge = true;
      break;

    case MergeType::BuildingToBuilding:
      //TODO: remove the returning of the merged id
      mergedId = BuildingMerger::merge(map);
      break;

    case MergeType::AreaToArea:
      mergedMap = _mergeAreas(map, plugin);
      scriptMerge = true;
      break;

    case MergeType::PoiToPolygon:
      //TODO: remove the returning of the merged id
      mergedId = PoiPolygonMerger::merge(map);
      break;

    default:
      throw HootException("Invalid merge type.");
  }

  //We only need to set the resulting merged element's status to conflated when the ScriptMerger
  //wasn't used, as it does so automatically.
  if (!scriptMerge)
  {
    LOG_VART(mergeId)
    LOG_VART(map->getElementCount());

    ElementPtr mergedElement = map->getElement(mergedId);
    mergedElement->setStatus(Status(Status::Conflated));
    mergedElement->getTags()[MetadataTags::HootStatus()] = "3";
    LOG_VART(mergedElement);
  }

  mergedMap = map;
  return mergedMap;
}

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map,
                                                                QString& scriptPath)
{
  MergeType mergeType;
  if (_containsOnlyPois(map))
  {
    mergeType = MergeType::PoiToPoi;
    scriptPath = ConfPath::search(toCpp<QString>("PoiGeneric.js"), "rules");
  }
  else if (_containsTwoBuildings(map))
  {
    mergeType = MergeType::BuildingToBuilding;
  }
  else if (_containsTwoOrMoreAreas(map))
  {
    mergeType = MergeType::AreaToArea;
    scriptPath = ConfPath::search(toCpp<QString>("Area.js"), "rules");
  }
  else if (_containsOnePolygonAndOneOrMorePois(map))
  {
    mergeType = MergeType::PoiToPolygon;
  }
  else
  {
    throw IllegalArgumentException(
      "Invalid inputs to element merger.  Inputs must be one of the following:" +
      "1) two or more POIs, 2) two buildings, 3) two or more areas, or " +
      "4) one or more POIs and one polygon.");
  }
  return mergeType;
}

bool ElementMergerJs::_containsOnlyPois(ConstOsmMapPtr map)
{
  if (map->getWayCount() > 0 || map->getRelationCount() > 0)
  {
    return false;
  }

  int poiCount = 0;
  NodeMap::const_iterator it = map->getNodes().begin();
  while (it != map->getNodes().end() && poiCount < 2)
  {
    const ConstNodePtr& node = it->second;
    if (OsmSchema::isPoi(node))
    {
      poiCount++;
    }
    ++it;
  }
  return poiCount >= 2;
}

bool ElementMergerJs::_containsTwoBuildings(ConstOsmMapPtr map)
{
  int buildingCount = 0;

  WayMap::const_iterator it = map->getWays().begin();
  while (it != map->getWays().end() && buildingCount < 3)
  {
    const ConstWayPtr& way = it->second;
    if (OsmSchema::isBuilding(way))
    {
      buildingCount++;
    }
    ++it;
  }
  if (buildingCount > 2)
  {
    return false;
  }

  RelationMap::const_iterator it = map->getRelations().begin();
  while (it != map->getRelations().end() && buildingCount < 3)
  {
    const ConstRelationPtr& relation = it->second;
    if (OsmSchema::isBuilding(relation))
    {
      buildingCount++;
    }
    ++it;
  }

  return buildingCount == 2;
}

bool ElementMergerJs::_containsTwoOrMoreAreas(ConstOsmMapPtr map)
{
  int areaCount = 0;

  WayMap::const_iterator it = map->getWays().begin();
  while (it != map->getWays().end() && areaCount < 2)
  {
    const ConstWayPtr& way = it->second;
    if (OsmSchema::isBuilding(way))
    {
      areaCount++;
    }
    ++it;
  }
  if (areaCount == 2)
  {
    return true;
  }

  RelationMap::const_iterator it = map->getRelations().begin();
  while (it != map->getRelations().end() && areaCount < 2)
  {
    const ConstRelationPtr& relation = it->second;
    if (OsmSchema::isBuilding(relation))
    {
      areaCount++;
    }
    ++it;
  }

  return areaCount >= 2;
}

bool ElementMergerJs::_containsOnePolygonAndOneOrMorePois(ConstOsmMapPtr map)
{
  if (map->getWayCount() == 0 && map->getRelationCount() == 0)
  {
    return false;
  }

  int poiCount = 0;
  NodeMap::const_iterator it = map->getNodes().begin();
  while (it != map->getNodes().end() && poiCount < 1)
  {
    const ConstNodePtr& node = it->second;
    if (PoiPolygonMatch::isPoi(node))
    {
      poiCount++;
    }
    ++it;
  }
  if (poiCount < 1)
  {
    return false;
  }

  int polyCount = 0;

  WayMap::const_iterator it = map->getWays().begin();
  while (it != map->getWays().end() && polyCount < 2)
  {
    const ConstWayPtr& way = it->second;
    if (PoiPolygonMatch::isPoly(way))
    {
      polyCount++;
    }
    ++it;
  }
  if (polyCount > 1)
  {
    return false;
  }

  RelationMap::const_iterator it = map->getRelations().begin();
  while (it != map->getRelations().end() && polyCount < 2)
  {
    const ConstRelationPtr& relation = it->second;
    if (PoiPolygonMatch::isPoly(relation))
    {
      polyCount++;
    }
    ++it;
  }

  return polyCount == 1;
}

OsmMapPtr ElementMergerJs::_mergeAreas(OsmMapPtr /*map*/, Persistent<Object> /*plugin*/)
{
  //TODO: finish
}

OsmMapPtr ElementMergerJs::_mergePois(OsmMapPtr map, Persistent<Object> plugin)
{
  // Got in Map with POIs A, B, C, D, E
  //
  // Make a set of pairs to indicate all are same object:
  //   A->B, A->C, A->D, A->E
  //
  // ...then pass those pairs one at a time through the merger, since it only merges pairs
  NodeMap nodes = map->getNodes();
  OsmMapPtr mergedMap(map);

  const ElementId firstId = ElementId::node(/*elementId*/-1); //TODO: fix
  LOG_TRACE("First ID: " << firstId.getId());
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (it->second->getId() != elementId)
    {
      const ConstNodePtr& n = it->second;

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(firstId, ElementId::node(n->getId())));

      // Now create scriptmerger, and invoke apply method which will apply merge
      // transformation, reducing the POIs down to one.
      ScriptMerger merger(script, plugin, matches);
      OsmMapPtr mergedMap(map);
      std::vector< std::pair< ElementId, ElementId > > replacedNodes;
      merger.apply(mergedMap, replacedNodes);

      if (replacedNodes.size() == 1)
      {
        LOG_TRACE(
          "POI merge: replacing node #" << replacedNodes[0].first.getId() <<
          " with updated version of node #" << replacedNodes[0].second.getId());
        mergedMap->replaceNode(replacedNodes[0].first.getId(), replacedNodes[0].second.getId());
      }
    }
  }
  return mergedMap;
}

}
