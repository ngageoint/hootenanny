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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MapIoJs.h"

// hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmGeoJsonReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

using namespace v8;
using namespace node;

namespace hoot
{

HOOT_JS_REGISTER(MapIoJs)

void MapIoJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  exports->Set(String::NewFromUtf8(current, "loadMap"),
               FunctionTemplate::New(current, loadMap)->GetFunction());
  exports->Set(String::NewFromUtf8(current, "loadMapFromString"),
               FunctionTemplate::New(current, loadMapFromString)->GetFunction());
  exports->Set(String::NewFromUtf8(current, "loadGeoJsonFromString"),
               FunctionTemplate::New(current, loadGeoJsonFromString)->GetFunction());
  exports->Set(String::NewFromUtf8(current, "loadMapFromStringPreserveIdAndStatus"),
               FunctionTemplate::New(current, loadMapFromStringPreserveIdAndStatus)->GetFunction());
  exports->Set(String::NewFromUtf8(current, "saveMap"),
               FunctionTemplate::New(current, saveMap)->GetFunction());
}

void MapIoJs::loadMap(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  try
  {
    OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());

    String::Utf8Value param1(args[1]->ToString());
    QString url = QString::fromUtf8(*param1);

    bool useFileId = true;
    if (args.Length() >= 3)
    {
      useFileId = args[2]->ToBoolean()->Value();
    }

    Status status = Status::Invalid;
    if (args.Length() >= 4)
    {
      status = (Status::Type)args[3]->ToInteger()->Value();
    }

    OsmMapReaderFactory::read(map->getMap(), url, useFileId, status);

    args.GetReturnValue().SetUndefined();
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

void MapIoJs::loadMapFromString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  QString mapXml = toCpp<QString>(args[1]);

  OsmXmlReader reader;
  if (args.Length() >= 3)
  {
    reader.setUseDataSourceIds(toCpp<bool>(args[2]));
  }
  Status status = Status::Invalid;
  if (args.Length() >= 4)
  {
    status = (Status::Type)args[3]->ToInteger()->Value();
    reader.setDefaultStatus(status);
  }
  reader.readFromString(mapXml, map->getMap());

  args.GetReturnValue().SetUndefined();
}

void MapIoJs::loadGeoJsonFromString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  QString mapGeoJson = toCpp<QString>(args[1]);

  OsmGeoJsonReader reader;
  if (args.Length() >= 3)
  {
    reader.setUseDataSourceIds(toCpp<bool>(args[2]));
  }
  Status status = Status::Invalid;
  if (args.Length() >= 4)
  {
    status = (Status::Type)args[3]->ToInteger()->Value();
    reader.setDefaultStatus(status);
  }
  reader.loadFromString(mapGeoJson, map->getMap());

  args.GetReturnValue().SetUndefined();
}


void MapIoJs::loadMapFromStringPreserveIdAndStatus(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  QString mapXml = toCpp<QString>(args[1]);

  OsmXmlReader reader;
  reader.setUseDataSourceIds(true);
  reader.setUseFileStatus(true);
  reader.readFromString(mapXml, map->getMap());

  args.GetReturnValue().SetUndefined();
}

void MapIoJs::saveMap(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapPtr map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject())->getMap();

  MapProjector::projectToWgs84(map);

  String::Utf8Value param1(args[1]->ToString());
  QString url = QString::fromUtf8(*param1);

  OsmMapWriterFactory::write(map, url);

  args.GetReturnValue().SetUndefined();
}

}
