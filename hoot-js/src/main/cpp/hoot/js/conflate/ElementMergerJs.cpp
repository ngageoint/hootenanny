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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
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
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/filters/BuildingCriterion.h>
#include <hoot/core/filters/NonBuildingAreaCriterion.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterion.h>
#include "PoiMergerJs.h"
#include "AreaMergerJs.h"

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
    //Context::Scope context_scope(current->GetCallingContext());
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
  //TODO: This error handling has been proven to not work in that it never seems to return the
  //error message to the nodejs calling service....making debugging a nightmare...or I'm just
  //doing something wrong here.  Either way, need to fix this.
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
  //merge target id won't be passed in for poi/poly, as the poi/poly merging picks the target
  //element itself
  if (mergeType != MergeType::PoiToPolygon)
  {
    mergeTargetId = _getMergeTargetFeatureId(map);
    LOG_VART(mergeTargetId);
  }

  //We're using a mix of generic conflation scripts and custom built classes to merge features
  //here, depending on the feature type.
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

  //By convention, we're setting the status of any element that gets merged with something to
  //conflated, even if its yet to be reviewed against something else.
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

  TagKeyCriterion mergeTagFilter(MetadataTags::HootMergeTarget());
  ElementIdSetVisitor idSetVis;
  FilteredVisitor filteredVis(mergeTagFilter, idSetVis);
  map->visitRo(filteredVis);
  const std::set<ElementId>& mergeTargetIds = idSetVis.getElementSet();
  assert(mergeTargetIds.size() == 1);

  return *mergeTargetIds.begin();
}

ElementMergerJs::MergeType ElementMergerJs::_determineMergeType(ConstOsmMapPtr map)
{
  //Making sure maps don't come in mixed, so callers don't have the expectation that they can merge
  //multiple feature types within the same map.  Any features in the map with types other than what
  //we know how to merge will just pass through.

  MergeType mergeType;

  const bool containsPolys = OsmUtils::containsPoiPolyPolys(map);
  LOG_VART(containsPolys);
  const bool containsAreas = OsmUtils::containsAreas(map); //non-building areas
  LOG_VART(containsAreas);
  const bool containsBuildings = OsmUtils::containsBuildings(map);
  LOG_VART(containsBuildings);
  const bool containsPois = OsmUtils::containsPois(map); //general poi definition
  LOG_VART(containsPois);
  if (OsmUtils::containsOnePolygonAndOnePoi(map))
  {
    mergeType = MergeType::PoiToPolygon;
  }
  else if (OsmUtils::containsTwoOrMorePois(map) && !containsPolys && !containsAreas &&
           !containsBuildings)
  {
    mergeType = MergeType::PoiToPoi;
  }
  else if (OsmUtils::containsTwoOrMoreBuildings(map) && !containsAreas && !containsPois)
  {
    mergeType = MergeType::BuildingToBuilding;
  }
  else if (OsmUtils::containsTwoOrMoreAreas(map) && !containsBuildings && !containsPois)
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
