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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MapIoJs.h"

// hoot
#include <hoot/core/elements/MapProjector.h>
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

void MapIoJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  exports->Set(String::NewFromUtf8(current, "loadMap"),
               FunctionTemplate::New(current, loadMap)->GetFunction(context).ToLocalChecked());
  exports->Set(String::NewFromUtf8(current, "loadMapFromString"),
               FunctionTemplate::New(current, loadMapFromString)->GetFunction(context).ToLocalChecked());
  exports->Set(String::NewFromUtf8(current, "loadMapFromStringPreserveIdAndStatus"),
               FunctionTemplate::New(current, loadMapFromStringPreserveIdAndStatus)->GetFunction(context).ToLocalChecked());
  exports->Set(String::NewFromUtf8(current, "saveMap"),
               FunctionTemplate::New(current, saveMap)->GetFunction(context).ToLocalChecked());
}

void MapIoJs::loadMap(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  try
  {
    OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());

    String::Utf8Value param1(current, args[1]->ToString(context).ToLocalChecked());
    QString url = QString::fromUtf8(*param1);

    bool useFileId = true;
    if (args.Length() >= 3)
    {
      useFileId = args[2]->ToBoolean(context).ToLocalChecked()->Value();
    }

    Status status = Status::Invalid;
    if (args.Length() >= 4)
    {
      status = (Status::Type)args[3]->ToInteger(context).ToLocalChecked()->Value();
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
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  QString mapXml = toCpp<QString>(args[1]);

  OsmXmlReader reader;
  if (args.Length() >= 3)
  {
    reader.setUseDataSourceIds(toCpp<bool>(args[2]));
  }
  Status status = Status::Invalid;
  if (args.Length() >= 4)
  {
    status = (Status::Type)args[3]->ToInteger(context).ToLocalChecked()->Value();
    reader.setDefaultStatus(status);
  }
  reader.readFromString(mapXml, map->getMap());

  args.GetReturnValue().SetUndefined();
}

void MapIoJs::loadMapFromStringPreserveIdAndStatus(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
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
  Local<Context> context = current->GetCurrentContext();

  OsmMapPtr map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked())->getMap();

  MapProjector::projectToWgs84(map);

  String::Utf8Value param1(current, args[1]->ToString(context).ToLocalChecked());
  QString url = QString::fromUtf8(*param1);

  OsmMapWriterFactory::write(map, url);

  args.GetReturnValue().SetUndefined();
}

}
