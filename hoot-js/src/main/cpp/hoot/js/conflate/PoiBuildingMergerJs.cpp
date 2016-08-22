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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include <set>
#include <utility>    // For std::pair
#include <hoot/js/SystemNodeJs.h>

#include "PoiBuildingMergerJs.h"

#include <hoot/js/OsmMapJs.h>
#include <boost/shared_ptr.hpp>
#include <v8.h>
#include <qstring.h>
#include <hoot/core/OsmMap.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/PluginContext.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>

namespace hoot
{

HOOT_JS_REGISTER(PoiBuildingMergerJs)


PoiBuildingMergerJs::PoiBuildingMergerJs()
{
}

PoiBuildingMergerJs::~PoiBuildingMergerJs()
{
}

void PoiBuildingMergerJs::Init(v8::Handle<v8::Object> exports)
{
  exports->Set(
    v8::String::NewSymbol("poiBuildingMerge"),
    v8::FunctionTemplate::New(jsPoiBuildingMerge)->GetFunction());
}

v8::Handle<v8::Value> PoiBuildingMergerJs::jsPoiBuildingMerge(const v8::Arguments& args)
{
  HandleScope scope;
  try
  {
    if (args.Length() > 3)
    {
      return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Expected three arguments for 'poiPolyMerge'.")));
    }

    // arg 1: map with the POI and building
    OsmMapJs* mapJs = node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
    OsmMapPtr map(mapJs->getMap());

    //there should be one poi node and one building way

    // arg 2: poi id
    const int poiId = toCpp<int>(args[1]);
    NodePtr poi = map->getNode(poiId);
    if (!poi.get() || !OsmSchema::getInstance().isPoi(*poi))
    {
      return
        v8::ThrowException(HootExceptionJs::create(
          IllegalArgumentException("Invalid POI passed to POI/building merger.  ID: " + poiId)));
    }
    ElementId poiElementId = ElementId::node(poiId);

    // arg3: building id
    const int buildingId = toCpp<int>(args[2]);
    //if a relation was passed in at all, the building must be that relation
    ElementId buildingElementId;
    if (map->getRelationMap().size() > 0)
    {
      RelationPtr building = map->getRelation(buildingId);
      if (!building.get() || !OsmSchema::getInstance().isBuilding(building))
      {
        return
          v8::ThrowException(HootExceptionJs::create(
            IllegalArgumentException(
              "Invalid building passed to POI/building merger.  ID: " + buildingId)));
      }
      buildingElementId = ElementId::relation(buildingId);
    }
    //otherwise, its a way
    else
    {
      WayPtr building = map->getWay(buildingId);
      if (!building.get() || !OsmSchema::getInstance().isBuilding(building))
      {
        return
          v8::ThrowException(HootExceptionJs::create(
            IllegalArgumentException(
              "Invalid building passed to POI/building merger.  ID: " + buildingId)));
      }
      buildingElementId = ElementId::way(buildingId);
    }

    std::set<std::pair<ElementId, ElementId> > pairs;
    pairs.insert(std::pair<ElementId, ElementId>(poiElementId, buildingElementId));
    PoiPolygonMerger merger(pairs);
    OsmMapPtr mergedMap(map);
    std::vector<std::pair<ElementId, ElementId> > replacedElements;
    merger.apply(mergedMap, replacedElements);

    //TODO: fix - I think the poi's always get merged into the building poly's, but I need to check
    LOG_VARD(replacedElements.size());
    if (replacedElements.size() == 1)
    {
      /*
      LOG_DEBUG("POI merge: replacing node #" << replacedNodes[0].first.getId() <<
        " with updated version of node #" << replacedNodes[0].second.getId() );
      */
      //mergedMap->replacedElements(
        //replacedElements[0].first.getId(), replacedElements[0].second.getId());
    }

    //Hand merged elements back to caller in OsmMap
    v8::Handle<v8::Object> returnMap = OsmMapJs::create(mergedMap);
    return scope.Close(returnMap);
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

}
