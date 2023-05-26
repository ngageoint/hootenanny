/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018-2023 Maxar (http://www.maxar.com/)
 */
#include "ElementMergerJs.h"

// Hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/StandalonePoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/RailwayOneToManySourceCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>

#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/merging/AreaMergerJs.h>
#include <hoot/js/conflate/merging/PoiMergerJs.h>
#include <hoot/js/conflate/merging/RailwayMergerJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// std
#include <utility>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementMergerJs)

void ElementMergerJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Maybe<bool> success = exports->Set(context, toV8("merge"), FunctionTemplate::New(current, merge)->GetFunction(context).ToLocalChecked());
  (void) success; // unused variable
}

void ElementMergerJs::merge(const FunctionCallbackInfo<Value>& args)
{
  LOG_INFO("Merging elements...");

  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  try
  {
    if (args.Length() != 1)
    {
      args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(IllegalArgumentException("Expected one argument passed to 'merge'."))));
      return;
    }

    OsmMapPtr map(node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked())->getMap());

    LOG_VART(map->getElementCount());
    _merge(map, current);
    LOG_VART(map->getElementCount());

    Local<Object> returnMap = OsmMapJs::create(map);
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
  case MergeType::Building:
    return "Building";
  case MergeType::PoiToPolygon:
    return "PoiToPolygon";
  case MergeType::Poi:
    return "Poi";
  case MergeType::Area:
    return "Area";
  case MergeType::Railway:
    return "Railway";
  default:
    throw IllegalArgumentException("Invalid merge type.");
  }
  return "";
}

void ElementMergerJs::_merge(OsmMapPtr map, Isolate* current)
{
  const MergeType mergeType = _determineMergeType(map);
  LOG_VART(_mergeTypeToString(mergeType));

  ElementId mergeTargetId;
  // The merge target id won't be passed in for poi/poly, as the poi/poly merging picks the target
  // element itself.
  if (mergeType != MergeType::PoiToPolygon)
  {
    mergeTargetId = _getMergeTargetFeatureId(map);
    LOG_VART(mergeTargetId);
  }

  // We're using a mix of generic conflation scripts and custom built classes to merge features
  // here, depending on the feature type, let the merger determine the merge target ID.
  switch (mergeType)
  {
  case MergeType::Building:
    mergeTargetId = BuildingMerger::merge(map, mergeTargetId);
    break;
  case MergeType::PoiToPolygon:
    mergeTargetId = PoiPolygonMerger::mergeOnePoiAndOnePolygon(map);
    break;
  case MergeType::Poi:
    mergeTargetId = PoiMergerJs::merge(map, mergeTargetId, current);
    break;
  case MergeType::Area:
    mergeTargetId = AreaMergerJs::merge(map, mergeTargetId, current);
    break;
  case MergeType::Railway:
    MapProjector::projectToPlanar(map);
    mergeTargetId = RailwayMergerJs::merge(map, mergeTargetId, current);
    SuperfluousNodeRemover::removeNodes(map);
    MapProjector::projectToWgs84(map);
    break;
  default:
    throw HootException("Invalid merge type.");
  }

  // By convention, we're setting the status of any element that gets merged with something to
  // conflated, even if its yet to be reviewed against something else.
  ElementPtr mergedElement = map->getElement(mergeTargetId);
  if (mergedElement)
  {
    mergedElement->setStatus(Status(Status::Conflated));
    mergedElement->getTags()[MetadataTags::HootStatus()] = "3";
    mergedElement->getTags().remove(MetadataTags::HootMergeTarget());
    LOG_VART(mergedElement);
  }
}

ElementId ElementMergerJs::_getMergeTargetFeatureId(ConstOsmMapPtr map)
{
  const bool containsAtLeastOneRailwayOneToManyMatchedElement = CriterionUtils::containsSatisfyingElements<RailwayOneToManySourceCriterion>(map);
  LOG_VART(containsAtLeastOneRailwayOneToManyMatchedElement);
  if (containsAtLeastOneRailwayOneToManyMatchedElement)
  {
    // If the input contains a rail feature matched with the one-to-many workflow, we do different
    // error checking here. We're looking for exactly one secondary feature with a custom tag added
    // by Railway.js and exactly one reference feature to merge tags into.

    const long numOneToManySecondaryMatchElements =
      (long)FilteredVisitor::getStat(std::make_shared<RailwayOneToManySourceCriterion>(), std::make_shared<ElementCountVisitor>(), map);
    if (numOneToManySecondaryMatchElements != 1)
      throw IllegalArgumentException("Input map for railway one-to-many merging must have exactly one qualifying secondary feature.");

    // _determineMergeType has already validated the input feature types, so just check status here.
    // We can merge into either a reference or already conflated feature.
    const long numStatus1Elements =
      (long)FilteredVisitor::getStat(
        std::make_shared<OrCriterion>(
          std::make_shared<StatusCriterion>(Status::Unknown1),
          std::make_shared<StatusCriterion>(Status::Conflated)),
        std::make_shared<ElementCountVisitor>(), map);
    if (numStatus1Elements != 1)
      throw IllegalArgumentException("Input map must have at exactly one feature with status=1 or status=3.");

    // Return the ref or already conflated feature's ID as the target ID.
    ConstElementPtr status1Element = MapUtils::getFirstElementWithStatus(map, Status::Unknown1);
    if (status1Element)
      return status1Element->getElementId();
    else
      return MapUtils::getFirstElementWithStatus(map, Status::Conflated)->getElementId();
  }

  // This logic is for all merge input validation except one-to-many rail. We should have exactly
  // one feature identified as the target and one or more features to merge into.

  const long numMergeTargets =
    (long)FilteredVisitor::getStat(
      std::make_shared<TagKeyCriterion>(MetadataTags::HootMergeTarget()),
      std::make_shared<ElementCountVisitor>(), map);
  LOG_VART(numMergeTargets);
  if (numMergeTargets != 1)
    throw IllegalArgumentException(QString("Input map must have exactly one feature marked with a %1 tag.").arg(MetadataTags::HootMergeTarget()));

  const long numNonMergeTargets =
    (long)FilteredVisitor::getStat(
      std::make_shared<NotCriterion>(
        std::make_shared<TagKeyCriterion>(MetadataTags::HootMergeTarget())),
      std::make_shared<ElementCountVisitor>(), map);
  LOG_VART(numMergeTargets);
  if (numNonMergeTargets < 1)
    throw IllegalArgumentException(QString("Input map must have at least one feature not marked with a %1 tag.").arg(MetadataTags::HootMergeTarget()));

  TagKeyCriterion mergeTagCrit(MetadataTags::HootMergeTarget());
  UniqueElementIdVisitor idSetVis;
  FilteredVisitor filteredVis(mergeTagCrit, idSetVis);
  map->visitRo(filteredVis);
  const std::set<ElementId>& mergeTargetIds = idSetVis.getElementSet();
  assert(mergeTargetIds.size() == 1);

  return *mergeTargetIds.begin();
}

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map)
{
  // Making sure maps don't come in mixed, so callers don't have the expectation that they can merge
  // multiple feature types within the same map. Technically, guess multiple type merging could be
  // done separately but was trying to keep things simple. The logic in this method could use some
  // cleaning up possibly.

  MergeType mergeType;

  // This should be able to be made cleaner.
  const bool containsPolys = CriterionUtils::containsSatisfyingElements<PoiPolygonPolyCriterion>(map);
  //non-building areas
  const bool containsAreas = CriterionUtils::containsSatisfyingElements<NonBuildingAreaCriterion>(map);
  const bool containsBuildings = CriterionUtils::containsSatisfyingElements<BuildingCriterion>(map);
  // You may occasionally see a POI node that is part of a linear or polygonal feature. That's
  // ok and we'll allow it.
  const bool containsStandalonePois = CriterionUtils::containsSatisfyingElements<StandalonePoiCriterion>(map);
  const bool containsRailways = CriterionUtils::containsSatisfyingElements<RailwayCriterion>(map);

  if (CriterionUtils::containsSatisfyingElements<PoiPolygonPoiCriterion>(map, 1, true) &&
      CriterionUtils::containsSatisfyingElements<PoiPolygonPolyCriterion>(map, 1, true) &&
      !containsRailways)
  {
    return MergeType::PoiToPolygon;
  }
  else if (CriterionUtils::containsSatisfyingElements<PoiCriterion>(map, 2) && !containsPolys &&
           !containsAreas && !containsBuildings && !containsRailways)
  {
    mergeType = MergeType::Poi;
  }
  else if (CriterionUtils::containsSatisfyingElements<BuildingCriterion>(map, 2) &&
           !containsAreas && !containsStandalonePois && !containsRailways)
  {
    mergeType = MergeType::Building;
  }
  else if (CriterionUtils::containsSatisfyingElements<NonBuildingAreaCriterion>(map, 2) &&
           !containsBuildings && !containsStandalonePois && !containsRailways)
  {
    mergeType = MergeType::Area;
  }
  else if (CriterionUtils::containsSatisfyingElements<RailwayCriterion>(map, 2, true) &&
           !containsBuildings && !containsStandalonePois && !containsAreas)
  {
    mergeType = MergeType::Railway;
  }
  else
  {
    // Update this error message as support feature types are added to this merger.
    const QString errorMsg =
      "Invalid inputs passed to the element merger. Input must contain only one "
      "combination of the following: 1) two or more POIs, 2) two or more buildings, 3)"
      "two or more areas, 4) one POI and one polygon, or 5) two railways";
    throw IllegalArgumentException(errorMsg);
  }

  return mergeType;
}

}
