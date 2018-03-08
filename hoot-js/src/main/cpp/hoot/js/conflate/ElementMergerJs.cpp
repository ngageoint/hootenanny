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
//#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
//#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/NonBuildingAreaCriterion.h>
#include <hoot/core/filters/PoiPolygonPoiCriterion.h>
#include <hoot/core/filters/PoiPolygonPolyCriterion.h>

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

    _validateMergeTargetElement(map, mergeType);

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

    OsmMapPtr mergedMap = _mergeElements(map, mergeType, plugin);

    Handle<Object> returnMap = OsmMapJs::create(mergedMap);
    args.GetReturnValue().Set(returnMap);
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

OsmMapPtr ElementMergerJs::_mergeElements(OsmMapPtr map, const MergeType& mergeType,
                                          Persistent<Object> plugin)
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
      //TODO: need to force merge target here
      //const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
      mergedId = BuildingMerger::merge(map);
      break;

    case MergeType::AreaToArea:
      mergedMap = _mergeAreas(map, plugin);
      scriptMerge = true;
      break;

    case MergeType::PoiToPolygon:
      //TODO: need to force merge target here
      //const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
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

ElementId ElementMergerJs::_getMergeTargetFeatureId(ConstOsmMapPtr /*map*/)
{
  //TODO: finish
  return ElementId();
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

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map,
                                                                QString& scriptPath)
{
  MergeType mergeType;
  if (_containsOnlyTwoOrMorePois(map))
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

  const ElementId mergeTargetId = _getMergeTargetFeatureId(map);
  LOG_TRACE("Merge target ID: " << mergeTargetId);
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (it->second->getId() != mergeTargetId)
    {
      const ConstNodePtr& n = it->second;

      std::set< std::pair< ElementId, ElementId> > matches;
      matches.insert(std::pair<ElementId,ElementId>(mergeTargetId, ElementId::node(n->getId())));

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
