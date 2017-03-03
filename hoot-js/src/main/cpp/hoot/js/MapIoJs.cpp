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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MapIoJs.h"

// hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/util/HootExceptionJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

#include "JsRegistrar.h"
#include "OsmMapJs.h"

namespace hoot
{
using namespace node;
using namespace v8;

class MapIoJs
{
public:

  static void Init(Handle<Object> exports)
  {
    exports->Set(String::NewSymbol("loadMap"), FunctionTemplate::New(loadMap)->GetFunction());
    exports->Set(String::NewSymbol("loadMapFromString"), FunctionTemplate::New(loadMapFromString)->
      GetFunction());
    exports->Set(String::NewSymbol("saveMap"), FunctionTemplate::New(saveMap)->GetFunction());
  }

  static Handle<Value> loadMap(const Arguments& args) {
    HandleScope scope;

    try
    {
      OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());

      v8::String::Utf8Value param1(args[1]->ToString());
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

      OsmMapReaderFactory::getInstance().read(map->getMap(), url, useFileId, status);
    }
    catch ( const HootException& e )
    {
      return v8::ThrowException(HootExceptionJs::create(e));
    }

    return scope.Close(Undefined());
  }

  static Handle<Value> loadMapFromString(const Arguments& args) {
    HandleScope scope;

    OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());

    QString mapXml = toCpp<QString>(args[1]);

    Status status = Status::Invalid;
    if (args.Length() >= 4)
    {
      status = (Status::Type)args[3]->ToInteger()->Value();
    }

    OsmXmlReader reader;
    bool useDataSourceIds = false;
    if (args.Length() >= 5)
    {
      useDataSourceIds = toCpp<bool>(args[4]);
      reader.setUseDataSourceIds(useDataSourceIds);
    }
    reader.readFromString(mapXml, map->getMap());

    return scope.Close(Undefined());
  }

  static Handle<Value> saveMap(const Arguments& args) {
    HandleScope scope;

    OsmMapPtr map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject())->getMap();

    MapProjector::projectToWgs84(map);

    v8::String::Utf8Value param1(args[1]->ToString());
    QString url = QString::fromUtf8(*param1);

    OsmMapWriterFactory::getInstance().write(map, url);

    return scope.Close(Undefined());
  }

};

HOOT_JS_REGISTER(MapIoJs)

}
