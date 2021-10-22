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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/RailwayOneToManyCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>

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
  exports->Set(
    context, toV8("merge"),
    FunctionTemplate::New(current, merge)->GetFunction(context).ToLocalChecked());
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
      args.GetReturnValue().Set(
        current->ThrowException(
          HootExceptionJs::create(
            IllegalArgumentException("Expected one argument passed to 'merge'."))));
      return;
    }

    OsmMapPtr map(
      node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked())->getMap());

    LOG_VART(map->getElementCount());
    _merge(map, current);
    LOG_VART(map->getElementCount());

    Local<Object> returnMap = OsmMapJs::create(map);
    args.GetReturnValue().Set(returnMap);
  }
  // This error handling has been proven to not work, as it never returns the error message to the
  // nodejs calling service....makes debugging very difficult. Need to fix: #2231. As a workaround,
  // use scripts/core/MergeElements.js to see log output during merging.
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
      return "Buildingg";
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
  // here, depending on the feature type.
  switch (mergeType)
  {
    case MergeType::Building:
      BuildingMerger::merge(map, mergeTargetId);
      break;

    case MergeType::PoiToPolygon:
      // POI/Poly always merges into the polygon and there's only one of them, so we let the
      // routine determine the merge target ID.
      mergeTargetId = PoiPolygonMerger::mergeOnePoiAndOnePolygon(map);
      break;

    case MergeType::Poi:
      PoiMergerJs::merge(map, mergeTargetId, current);
      break;

    case MergeType::Area:
      AreaMergerJs::merge(map, mergeTargetId, current);
      break;

    case MergeType::Railway:
      MapProjector::projectToPlanar(map);
      RailwayMergerJs::merge(map, mergeTargetId, current);
      SuperfluousNodeRemover::removeNodes(map);
      MapProjector::projectToWgs84(map);
      break;

    default:
      throw HootException("Invalid merge type.");
  }

  // By convention, we're setting the status of any element that gets merged with something to
  // conflated, even if its yet to be reviewed against something else.
  ElementPtr mergedElement = map->getElement(mergeTargetId);
  assert(mergedElement);
  mergedElement->setStatus(Status(Status::Conflated));
  mergedElement->getTags()[MetadataTags::HootStatus()] = "3";
  mergedElement->getTags().remove(MetadataTags::HootMergeTarget());
  LOG_VART(mergedElement);
}

ElementId ElementMergerJs::_getMergeTargetFeatureId(ConstOsmMapPtr map)
{
  // TODO
  const bool containsAtLeastOneRailwayOneToManyMatchedElement =
    CriterionUtils::containsSatisfyingElements<RailwayOneToManyCriterion>(map);
  LOG_VART(containsAtLeastOneRailwayOneToManyMatchedElement);
  if (containsAtLeastOneRailwayOneToManyMatchedElement)
  {
    const long numOneToManySecondaryMatchElements =
      (long)FilteredVisitor::getStat(
        std::make_shared<RailwayOneToManyCriterion>(),
        std::make_shared<ElementCountVisitor>(), map);
    if (numOneToManySecondaryMatchElements != 1)
    {
      throw IllegalArgumentException(
        "Input map for railway one to many merging must have exactly one qualifying "
        "secondary feature.");
    }

    // _determineMergeType has already validated feature type, so just check status here.
    const long numStatus1Elements =
      (long)FilteredVisitor::getStat(
        std::make_shared<OrCriterion>(
          std::make_shared<StatusCriterion>(Status::Unknown1),
          std::make_shared<StatusCriterion>(Status::Conflated)),
        std::make_shared<ElementCountVisitor>(), map);
    if (numStatus1Elements < 1)
    {
      throw IllegalArgumentException(
        "Input map must have at least one feature with status=1 or status=3.");
    }

    ConstElementPtr status1Element = MapUtils::getFirstElementWithStatus(map, Status::Unknown1);
    if (status1Element)
    {
      return status1Element->getElementId();
    }
    else
    {
      return MapUtils::getFirstElementWithStatus(map, Status::Conflated)->getElementId();
    }
  }

  const long numMergeTargets =
    (long)FilteredVisitor::getStat(
      std::make_shared<TagKeyCriterion>(MetadataTags::HootMergeTarget()),
      std::make_shared<ElementCountVisitor>(), map);
  LOG_VART(numMergeTargets);
  if (numMergeTargets != 1)
  {
    throw IllegalArgumentException(
      "Input map must have exactly one feature marked with a " + MetadataTags::HootMergeTarget() +
      " tag.");
  }

  const long numNonMergeTargets =
    (long)FilteredVisitor::getStat(
      std::make_shared<NotCriterion>(
        std::make_shared<TagKeyCriterion>(MetadataTags::HootMergeTarget())),
      std::make_shared<ElementCountVisitor>(), map);
  LOG_VART(numMergeTargets);
  if (numNonMergeTargets < 1)
  {
    throw IllegalArgumentException(
      "Input map must have at least one feature not marked with a " +
      MetadataTags::HootMergeTarget() + " tag.");
  }

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
  // multiple feature types within the same map. After the initial feature requirements are
  // satisfied, any other features in the map with types other than what we know how to merge
  // should just pass through.

  MergeType mergeType;

  // This should be able to be made cleaner.
  const bool containsPolys =
    CriterionUtils::containsSatisfyingElements<PoiPolygonPolyCriterion>(map);
  LOG_VART(containsPolys);
  //non-building areas
  const bool containsAreas =
    CriterionUtils::containsSatisfyingElements<NonBuildingAreaCriterion>(map);
  LOG_VART(containsAreas);
  const bool containsBuildings =
    CriterionUtils::containsSatisfyingElements<BuildingCriterion>(map);
  LOG_VART(containsBuildings);
  const bool containsPois =
    CriterionUtils::containsSatisfyingElements<PoiCriterion>(map); // general poi definition
  LOG_VART(containsPois);
  const bool containsRailways = CriterionUtils::containsSatisfyingElements<RailwayCriterion>(map);
  LOG_VART(containsRailways);
  LOG_VART(CriterionUtils::containsSatisfyingElements<RailwayCriterion>(map, 2));

  if (CriterionUtils::containsSatisfyingElements<PoiPolygonPoiCriterion>(map, 1, true) &&
      CriterionUtils::containsSatisfyingElements<PoiPolygonPolyCriterion>(map, 1, true) &&
      !containsRailways)
  {
    mergeType = MergeType::PoiToPolygon;
  }
  else if (CriterionUtils::containsSatisfyingElements<PoiCriterion>(map, 2) && !containsPolys &&
           !containsAreas && !containsBuildings && !containsRailways)
  {
    mergeType = MergeType::Poi;
  }
  else if (CriterionUtils::containsSatisfyingElements<BuildingCriterion>(map, 2) &&
           !containsAreas && !containsPois && !containsRailways)
  {
    mergeType = MergeType::Building;
  }
  else if (CriterionUtils::containsSatisfyingElements<NonBuildingAreaCriterion>(map, 2) &&
           !containsBuildings && !containsPois && !containsRailways)
  {
    mergeType = MergeType::Area;
  }
  // TODO: if the POI is standalone, no dice; if part of the rail, then ok
  else if (CriterionUtils::containsSatisfyingElements<RailwayCriterion>(map, 2) &&
           !containsBuildings && /*!containsPois &&*/ !containsAreas)
  {
    mergeType = MergeType::Railway;
  }
  else
  {
    // Update this error message as support feature types are added to this merger.
    const QString errorMsg =
      QString("Invalid inputs passed to the element merger. Inputs must contain only one ") +
      QString("combination of the following: 1) two or more POIs, 2) two or more buildings, 3)") +
      QString("two or more areas, 4) one POI and one polygon, or 5) two or more railways");
    throw IllegalArgumentException(errorMsg);
  }

  return mergeType;
}

}
