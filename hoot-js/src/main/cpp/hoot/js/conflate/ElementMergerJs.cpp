#include "ElementMergerJs.h"

// Hoot
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/ElementIdSetVisitor.h>
#include <hoot/core/util/MapUtils.h>
#include "PoiMergerJs.h"
#include "AreaMergerJs.h"

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
    LOG_ERROR(e.getWhat());
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

  ElementId mergeTargetId;
  if (mergeType != MergeType::PoiToPolygon)
  {
    mergeTargetId = _getMergeTargetFeatureId(map);
    LOG_VART(mergeTargetId);
  }

  bool scriptMerge = false;
  switch (mergeType)
  {
    case MergeType::BuildingToBuilding:
      BuildingMerger::mergeBuildings(map, mergeTargetId);
      break;

    case MergeType::PoiToPolygon:
      mergeTargetId = PoiPolygonMerger::mergePoiAndPolygon(map);
      break;

    case MergeType::PoiToPoi:
      PoiMergerJs::mergePois(map, mergeTargetId, current);
      scriptMerge = true;
      break;

    case MergeType::AreaToArea:
      AreaMergerJs::mergeAreas(map, mergeTargetId, current);
      scriptMerge = true;
      break;

    default:
      throw HootException("Invalid merge type.");
  }
  LOG_VART(scriptMerge);

  ElementPtr mergedElement = map->getElement(mergeTargetId);
  mergedElement->setStatus(Status(Status::Conflated));
  mergedElement->getTags()[MetadataTags::HootStatus()] = "3";
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
  //For POI to polygon conflation we always merge the POI into the polygon, so we don't need the
  //merge target ID and will ignore it if it is there.
  if (mergeType != MergeType::PoiToPolygon)
  {
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
  }
}

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map)
{
  MergeType mergeType;
  //Making sure maps don't come in mixed, so callers don't have the expectation that they can merge
  //multiple feature types within the same map.  Any features existing in the map outside of what
  //we know how to merge will just pass through.
  const bool containsPolys = MapUtils::containsPolys(map);   //this is the poi/poly definition of poly
  LOG_VART(containsPolys);
  const bool containsAreas = MapUtils::containsAreas(map); //non-building areas
  LOG_VART(containsAreas);
  const bool containsBuildings = MapUtils::containsBuildings(map);
  LOG_VART(containsBuildings);
  const bool containsPois = MapUtils::containsPois(map); //general poi definition
  LOG_VART(containsPois);
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
    const QString errorMsg =
      QString("Invalid inputs passed to the element merger.  Inputs must contain only one ") +
      QString("combination of the following:  1) two or more POIs, 2) two or more buildings, 3)") +
      QString("two or more areas, or 4) one POI and one polygon");
    //LOG_ERROR(errorMsg);
    throw IllegalArgumentException(errorMsg);
  }
  return mergeType;
}

}
