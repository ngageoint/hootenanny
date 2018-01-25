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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
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

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(PoiPolygonMergerJs)


PoiPolygonMergerJs::PoiPolygonMergerJs()
{
}

PoiPolygonMergerJs::~PoiPolygonMergerJs()
{
}

void PoiPolygonMergerJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  exports->Set(String::NewFromUtf8(current, "poiPolyMerge"),
               FunctionTemplate::New(current, jsPoiPolyMerge)->GetFunction());
}

void PoiPolygonMergerJs::jsPoiPolyMerge(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  try
  {
    if (args.Length() != 1)
    {
      args.GetReturnValue().Set(current->ThrowException(
            HootExceptionJs::create(IllegalArgumentException("Expected one argument for 'poiPolyMerge'."))));
      return;
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

    args.GetReturnValue().Set(OsmMapJs::create(map));
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}
