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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PoiPolygonMergerJs.h"

#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>

namespace hoot
{

HOOT_JS_REGISTER(PoiPolygonMergerJs)


PoiPolygonMergerJs::PoiPolygonMergerJs()
{
}

PoiPolygonMergerJs::~PoiPolygonMergerJs()
{
}

void PoiPolygonMergerJs::Init(v8::Handle<v8::Object> exports)
{
  exports->Set(
    v8::String::NewSymbol("poiPolyMerge"),
    v8::FunctionTemplate::New(jsPoiPolyMerge)->GetFunction());
}

v8::Handle<v8::Value> PoiPolygonMergerJs::jsPoiPolyMerge(const v8::Arguments& args)
{
  HandleScope scope;
  try
  {
    if (args.Length() != 1)
    {
      return
        v8::ThrowException(
          HootExceptionJs::create(
            IllegalArgumentException("Expected one argument for 'poiPolyMerge'.")));
    }

    OsmMapJs* mapJs = node::ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
    OsmMapPtr map(mapJs->getMap());
    LOG_VARD(map->getElementCount());

    const ElementId polyId = PoiPolygonMerger::merge(map);
    LOG_VARD(map->getElementCount());

    //set the poly to have a conflated status (what the poi was merged into); status expected by
    //the client
    ElementPtr polyElement = map->getElement(polyId);
    polyElement->setStatus(Status(Status::Conflated));
    polyElement->getTags()[MetadataTags::HootStatus()] = "3";

    if (Log::getInstance().isDebugEnabled())
    {
      LOG_DEBUG("Writing debug map...");
      OsmMapPtr debug(new OsmMap(map));
      MapProjector::projectToWgs84(debug);
      //if you're testing this from the mocha test, you may need to put an absolute path here for
      //the output instead
      //OsmMapWriterFactory::write(debug, ConfigOptions().getDebugMapFilename());
    }

    v8::Handle<v8::Object> returnMap = OsmMapJs::create(map);
    return scope.Close(returnMap);
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

}
