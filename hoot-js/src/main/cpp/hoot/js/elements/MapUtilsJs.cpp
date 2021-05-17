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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "MapUtilsJs.h"

// hoot
#include <hoot/core/elements/MapUtils.h>
#include <hoot/core/util/Factory.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/io/DataConvertJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(MapUtilsJs)

void MapUtilsJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> obj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "MapUtils"), obj);

  obj->Set(String::NewFromUtf8(current, "getFirstElementWithTag"),
           FunctionTemplate::New(current, getFirstElementWithTag)->GetFunction(context).ToLocalChecked());
  obj->Set(String::NewFromUtf8(current, "getFirstElementWithNote"),
           FunctionTemplate::New(current, getFirstElementWithNote)->GetFunction(context).ToLocalChecked());
}

void MapUtilsJs::getFirstElementWithTag(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapPtr map = toCpp<OsmMapPtr>(args[0]);
  const QString key = toCpp<QString>(args[1]);
  const QString val = toCpp<QString>(args[2]);
  ElementType elementType = ElementType::Unknown;
  if (args.Length() == 4)
  {
    elementType = ElementType::fromString(toCpp<QString>(args[3]));
  }

  ElementPtr element = MapUtils::getFirstElementWithTag(map, key, val, elementType);

  args.GetReturnValue().Set(toV8(element));
}

void MapUtilsJs::getFirstElementWithNote(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapPtr map = toCpp<OsmMapPtr>(args[0]);
  const QString noteVal = toCpp<QString>(args[1]);
  ElementType elementType = ElementType::Unknown;
  if (args.Length() == 3)
  {
    elementType = ElementType::fromString(toCpp<QString>(args[2]));
  }

  ElementPtr element = MapUtils::getFirstElementWithNote(map, noteVal, elementType);

  args.GetReturnValue().Set(toV8(element));
}

}
