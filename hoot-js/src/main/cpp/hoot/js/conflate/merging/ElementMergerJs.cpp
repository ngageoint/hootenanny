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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementMergerJs.h"

// Hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/conflate/merging/AreaMergerJs.h>
#include <hoot/js/conflate/merging/PoiMergerJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QString>
#include <QList>

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
  exports->Set(String::NewFromUtf8(current, "mergeElements"),
               FunctionTemplate::New(current, mergeElements)->GetFunction());
}

void ElementMergerJs::mergeElements(const FunctionCallbackInfo<Value>& args)
{
  LOG_INFO("Merging elements...");

  Isolate* current = args.GetIsolate();
  try
  {
    HandleScope scope(current);
    if (args.Length() != 1)
    {
      args.GetReturnValue().Set(
        current->ThrowException(
          HootExceptionJs::create(
            IllegalArgumentException("Expected one argument to 'mergeElements'."))));
      return;
    }

    OsmMapPtr map(node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject())->getMap());
    LOG_VART(map->getElementCount());
    _mergeElements(map, current);
    LOG_VART(map->getElementCount());

    Handle<Object> returnMap = OsmMapJs::create(map);
    args.GetReturnValue().Set(returnMap);
  }
  // This error handling has been proven to not work as it never returns the error message to the
  // nodejs calling service....makes debugging very difficult. Need to fix: #2231. As a workaround,
  // use scripts/core/MergeElements.js to see log output duriing merging.
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
      throw IllegalArgumentException("Invalid merge type.");
  }
  return "";
}

void ElementMergerJs::_mergeElements(OsmMapPtr map, Isolate* current)
{  
  const MergeType mergeType = _determineMergeType(map);
  LOG_VART(_mergeTypeToString(mergeType));

  ElementId mergeTargetId;
  // merge target id won't be passed in for poi/poly, as the poi/poly merging picks the target
  // element itself
  if (mergeType != MergeType::PoiToPolygon)
  {
    mergeTargetId = _getMergeTargetFeatureId(map);
    LOG_VART(mergeTargetId);
  }

  // We're using a mix of generic conflation scripts and custom built classes to merge features
  // here, depending on the feature type.
  bool scriptMerge = false;
  switch (mergeType)
  {
    case MergeType::BuildingToBuilding:
      BuildingMerger::mergeBuildings(map, mergeTargetId);
      break;

    case MergeType::PoiToPolygon:
      // POI/Poly always merges into the polygon and there's only one of them, so we let the
      // routine determine the merge target ID.
      mergeTargetId = PoiPolygonMerger::mergeOnePoiAndOnePolygon(map);
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
  // multiple feature types within the same map.  After the initial feature requirements are
  // satisified, any other features in the map with types other than what we know how to merge
  // should just pass through.

  MergeType mergeType;

  const bool containsPolys = CriterionUtils::contains<PoiPolygonPolyCriterion>(map);
  LOG_VART(containsPolys);
  //non-building areas
  const bool containsAreas = CriterionUtils::contains<NonBuildingAreaCriterion>(map);
  LOG_VART(containsAreas);
  const bool containsBuildings = CriterionUtils::contains<BuildingCriterion>(map);
  LOG_VART(containsBuildings);
  const bool containsPois = CriterionUtils::contains<PoiCriterion>(map); //general poi definition
  LOG_VART(containsPois);
  if (CriterionUtils::contains<PoiPolygonPoiCriterion>(map, 1, true) &&
      CriterionUtils::contains<PoiPolygonPolyCriterion>(map, 1, true))
  {
    mergeType = MergeType::PoiToPolygon;
  }
  else if (CriterionUtils::contains<PoiCriterion>(map, 2) && !containsPolys && !containsAreas &&
           !containsBuildings)
  {
    mergeType = MergeType::PoiToPoi;
  }
  else if (CriterionUtils::contains<BuildingCriterion>(map, 2) && !containsAreas && !containsPois)
  {
    mergeType = MergeType::BuildingToBuilding;
  }
  else if (CriterionUtils::contains<NonBuildingAreaCriterion>(map, 2) && !containsBuildings &&
           !containsPois)
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
